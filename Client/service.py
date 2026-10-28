import asyncio
import chat_pb2
import chat_pb2_grpc
import grpc
import queue
import threading

from message import Message
from retry import Retry
from tkinter import messagebox


# The service allows communication with the Chat server.
class Service:
    def __init__(self, client_id, url):
        self._client_id = client_id
        self._url = url

        # We have a message queue which will contain messages
        # sent from the Chat server. We can pop messages off this queue
        # as they arrive.
        self.message_queue = queue.Queue()

        # The Chat server sends back notifications after we subscribe.
        self._notification_stream = None
        self._notification_handler_task = None

        # We need a separate event loop for the asyncio to prevent
        # interfering with the Tk event loop.
        self._loop = asyncio.new_event_loop()
        threading.Thread(
            target=self._asyncio_thread,
            daemon=True
        ).start()

    def send_message(self, text):
        asyncio.run_coroutine_threadsafe(
            self._send_message(text), self._loop
        )

    async def _notification_handler(self):
        # We check notifications for new messages.
        async for notification in self._notification_stream:
            message = Message(
                notification.client_message.client_id,
                notification.client_message.text)
            self.message_queue.put(message)

    async def _subscribe(self):
        # Subscribe to the Chat server and ensure we can read
        # the stream of notifications sent back.
        request = chat_pb2.SubscribeRequest(client_id=self._client_id)
        self._notification_stream = self.stub.Subscribe(request)
        await self._notification_handler()

    async def _start(self):
        channel = grpc.aio.insecure_channel(self._url)
        self.stub = chat_pb2_grpc.ChatServiceStub(channel)
        try:
            await Retry(self._subscribe, 5, 1).try_rpc()
        except grpc.aio.AioRpcError as e:
            messagebox.showerror("Error", f"Failed to join server:\n\n{e}")

    def _asyncio_thread(self):
        asyncio.set_event_loop(self._loop)
        self._loop.run_until_complete(self._start())

    async def _send_message(self, text):
        request = chat_pb2.ClientMessage(
            client_id=self._client_id,
            text=text
        )
        await self.stub.SendMessage(request)

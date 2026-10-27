import asyncio
import chat_pb2
import chat_pb2_grpc
import grpc
import queue
import threading

from message import Message


class Service:
    def __init__(self, client_id, url):
        self._client_id = client_id
        self._url = url

        self.message_queue = queue.Queue()

        self._notification_stream = None
        self._notification_handler_task = None

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
        try:
            async for notification in self._notification_stream:
                message = Message(
                    notification.client_message.client_id,
                    notification.client_message.text)
                self.message_queue.put(message)
        except grpc.aio.AioRpcError as e:
            print(f"Subscription ended: {e}")

    async def _start(self):
        channel = grpc.aio.insecure_channel(self._url)
        self.stub = chat_pb2_grpc.ChatServiceStub(channel)

        request = chat_pb2.SubscribeRequest(client_id=self._client_id)
        self._notification_stream = self.stub.Subscribe(request)
        self._notification_handler_task = asyncio.create_task(
            self._notification_handler()
        )

    def _asyncio_thread(self):
        asyncio.set_event_loop(self._loop)
        self._loop.run_until_complete(self._start())
        self._loop.run_forever()

    async def _send_message(self, text):
        request = chat_pb2.ClientMessage(
            client_id=self._client_id,
            text=text
        )
        await self.stub.SendMessage(request)

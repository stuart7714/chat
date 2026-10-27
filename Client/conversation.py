import queue
import tkinter as tk
import tkinter.font as tkfont


# We show all the chat messages as a conversation history.
class Conversation:
    def __init__(self, main_frame, service):
        self.service = service

        self.frame = tk.Frame(main_frame)
        self.frame.pack(side=tk.TOP, fill=tk.BOTH, expand=True)

        history_frame = tk.Frame(self.frame)
        history_frame.pack(fill=tk.BOTH, expand=True)

        chat_scrollbar = tk.Scrollbar(history_frame, orient=tk.VERTICAL)
        chat_scrollbar.pack(side=tk.RIGHT, fill=tk.Y)

        self.chat_text = tk.Text(history_frame, wrap=tk.WORD,
                                 yscrollcommand=chat_scrollbar.set)
        self.chat_text.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)
        self.chat_text.tag_configure("normal", font=("TkFixedFont", 10))
        self.chat_text.tag_configure("bold", font=("TkFixedFont", 10, "bold"))

        chat_scrollbar.configure(command=self.chat_text.yview)

        # We check for new messages every 100 ms.
        self.frame.after(100, self.process_client_messages)

    def process_client_messages(self):
        # The service has a message queue which we poll to see if there
        # are new messages which need to be displayed.
        try:
            while True:
                client_message = self.service.message_queue.get_nowait()
                self.chat_text.insert(
                    tk.END, f"{client_message.client_id}: ", "bold")
                self.chat_text.insert(
                    tk.END, f"{client_message.text}\n", "normal")
                self.chat_text.see(tk.END)
        except queue.Empty:
            pass

        # We check for new messages every 100 ms.
        self.frame.after(100, self.process_client_messages)

import tkinter as tk


# The user enters messages to send to all users connected to the Chat server.
class Input:
    def __init__(self, main_frame, service):
        self.service = service

        frame = tk.Frame(main_frame)
        frame.pack(fill=tk.X, pady=(5, 0))

        self.message_entry = tk.Entry(frame)
        self.message_entry.pack(side=tk.LEFT, fill=tk.X,
                                expand=True, padx=(0, 5))

        send_button = tk.Button(frame, text="Send")
        send_button.pack(side=tk.RIGHT)

        # The user can press return or press the send button
        # to send a message.
        send_button.configure(command=self.send_message)
        self.message_entry.bind("<Return>", self.send_message)
        self.message_entry.focus_set()

    def send_message(self, event=None):
        # We send the message to the Chat server via the service.
        text = self.message_entry.get()
        self.message_entry.delete(0, tk.END)
        self.service.send_message(text)

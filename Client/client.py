# python -m grpc_tools.protoc -I="../Proto" --python_out=. --grpc_python_out=. "../Proto/chat.proto"

import argparse
import tkinter as tk

from conversation import Conversation
from input import Input
from service import Service


# The Chat client needs a client-id to identify the user (e.g. "Alice" or "Bob")
# and a URL to connect to the Chat server (e.g. "localhost:50051")
parser = argparse.ArgumentParser(
    description="A simple Chat client"
)

parser.add_argument("--client-id", required=True, help="The client identifier")
parser.add_argument("--url", required=True, help="The URL of the chat server")
args = parser.parse_args()

root = tk.Tk()
root.title("Chat")
root.geometry("600x500")

main_frame = tk.Frame(root, padx=5, pady=5)
main_frame.pack(fill=tk.BOTH, expand=True)

# The service allows communication with the Chat server.
service = Service(args.client_id, args.url)

# We show all the chat messages as a conversation history.
conversation = Conversation(main_frame, service)

# The user enters messages to send to all users connected to the Chat server.
input = Input(main_frame, service)

root.mainloop()

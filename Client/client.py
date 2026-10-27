# python -m grpc_tools.protoc -I="../Proto" --python_out=. --grpc_python_out=. "../Proto/chat.proto"

import argparse
import tkinter as tk

from conversation import Conversation
from input import Input
from service import Service


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

service = Service(args.client_id, args.url)
conversation = Conversation(main_frame, service)
input = Input(main_frame, service)

root.mainloop()

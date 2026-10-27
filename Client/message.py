# A message is a piece of text sent by a specific user.
class Message:
    def __init__(self, client_id, text):
        self.client_id = client_id
        self.text = text

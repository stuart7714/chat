import asyncio
import grpc


# We can retry calling a RPC a number of times.
# We have exponential backoff for the attempts.
class Retry:
    def __init__(self, rpc, max_retries, initial_delay):
        self.rpc = rpc
        self.max_retries = max_retries
        self.initial_delay = initial_delay

    async def try_rpc(self):
        delay = self.initial_delay

        for attempt in range(self.max_retries + 1):
            try:
                print("Call RPC")
                return await self.rpc()

            except grpc.aio.AioRpcError as e:
                retries_remaining = self.max_retries - attempt

                if retries_remaining == 0:
                    print(f"RPC failed; aborting: {e.code()}")
                    raise

                print(
                    f"RPC failed ({e.code()}); "
                    f"{retries_remaining} retries remaining"
                )

                print(f"Waiting {delay} seconds before retrying")
                await asyncio.sleep(delay)
                delay *= 2

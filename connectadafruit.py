# To test the working of adafruit io communication

from Adafruit_IO import RequestError, Client, Feed

ADAFRUIT_IO_USERNAME = "AyushJam"
ADAFRUIT_IO_KEY = "<your aio key here>"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
data = 378

try:
    ges = aio.feeds("gesturebot")
except RequestError:
    ges_feed = Feed(name="GestureBot")
    ges = aio.create_feed(ges_feed)

aio.send("gesturebot", data)

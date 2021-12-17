import std/[
  strutils,
]

const InputText = staticRead("Input.txt")

var prevNumber: ref uint = nil
var count = 0

for line in InputText.split("\n"):
  try:
    let number = line.parseUInt()
    if prevNumber == nil:
      prevNumber.new()
    elif prevNumber[] < number:
      count += 1
    prevNumber[] = number
  except ValueError:
    discard

echo count

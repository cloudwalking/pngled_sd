#bin/python

from PIL import Image
import numpy
import sys

def printStringToFile(string, filename):
  file = open(filename, "w");
  file.write(string)
  file.close()

def constructDataFileText(image):
  # Format:
  # 0x60 0x60 <pixels> <frames> <frames per second>
  string = chr(0x60)
  string += chr(0x60)
  
  pixels = image.shape[0]
  frames = image.shape[1]
  
  string += chr(pixels)
  string += chr(frames)
  
  pixels_per_sec = 25
  string += chr(pixels_per_sec)
  
  for i in range(0, frames):
    for row in image[ : ,i]:
      string += chr(int(row[0]))
      string += chr(int(row[1]))
      string += chr(int(row[2]))

  return string

if __name__ == "__main__":
  name = sys.argv[1]

  image = Image.open(name + ".png");
  png = numpy.asarray(image)
  
  print png.shape

  data = constructDataFileText(png);
  printStringToFile(data, name + ".dat");

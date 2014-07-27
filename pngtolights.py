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
    column = numpy.fliplr(image [:,i])
    for row in column:
      string += chr(int(row[0]))
      string += chr(int(row[1]))
      string += chr(int(row[2]))

  return string
  
def make(filename):
  image = Image.open(filename);
  png = numpy.asarray(image)

  print filename + "  " + str(png.shape)

  data = constructDataFileText(png);
  filename = filename[:-4] + ".dat";
  printStringToFile(data, filename);

if __name__ == "__main__":
  
  if len(sys.argv) < 2:
    print "Usage: python " + sys.argv[0] + " <image.png> [image2.png ...]"
  else:
    for arg in sys.argv[1:]:
      make(arg)

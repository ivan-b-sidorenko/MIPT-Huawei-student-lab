#!/usr/bin/env python3
import numpy as np
import argparse
from scipy import signal


def main():
    parser = argparse.ArgumentParser(description='Genrator of matrix for test')
    parser.add_argument('batches', metavar='N', type=int, help='batches amount')
    parser.add_argument('channels', metavar='C', type=int, help='channels amount')
    parser.add_argument('rows', metavar='H', type=int, help='rows amount')
    parser.add_argument('cols', metavar='W', type=int, help='cols amount')
    parser.add_argument('kern_channels', metavar='KCH', type=int, help='kernel channles amount')
    parser.add_argument('kern_am', metavar='KAM', type=int, help='kernels amount')
    parser.add_argument('to', metavar='TO', type=int, help='end value for generate numbers')
    parser.add_argument('fr', metavar='FROM', type=int, help='start value for generate numbers')

    args = parser.parse_args()

    if args.batches <= 0 or args.channels <= 0 or args.rows <= 0 or args.cols <= 0:
      print('Incorrect size. Try again')
      return
    if args.to < args.fr:
      args.to, args.fr = args.fr, args.to

    kern_size = (args.kern_am, args.kern_channels, 4, 4)
    tensor = np.random.randint(args.fr, args.to, size=(args.batches, args.channels, args.rows, args.cols))
    kerns = np.random.randint(args.fr, args.to, size=kern_size)
    
    print(args.batches, args.channels, args.rows, args.cols)
  
    for i in tensor:
      for j in i:
        for k in j:
          for w in k:
            print(w, end=' ')
          print()
        print()
      print()

    print(kern_size[0])

    for i in kerns:
      print(kern_size[1], kern_size[2], kern_size[3])
      for j in i:
        for k in j:
          for w in k:
            print(w, end=' ')
          print()
        print()
      print()
    
    conv = []
    c_shp = signal.correlate(tensor[0], kerns[0], mode='valid').shape

    for img in tensor:
      el = np.zeros(c_shp, dtype=int)

      for kern in kerns:
        el += signal.correlate(img, kern, mode='valid')
      conv.append(el)

    print(len(conv), end=' ')

    for sh in conv[0].shape:
      print(sh, end=' ')
    print()

    for i in conv:
      for j in i:
        for k in j:
          for w in k:
            print(w, end=' ')
          print()
        print()
      print()


if __name__ == '__main__':
    main()
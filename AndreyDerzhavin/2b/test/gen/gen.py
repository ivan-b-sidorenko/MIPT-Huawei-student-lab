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
    parser.add_argument('to', metavar='TO', type=int, help='end value for generate numbers')
    parser.add_argument('fr', metavar='FROM', type=int, help='start value for generate numbers')

    args = parser.parse_args()

    if args.batches <= 0 or args.channels <= 0 or args.rows <= 0 or args.cols <= 0:
      print('Incorrect size. Try again')
      return
    if args.to < args.fr:
      args.to, args.fr = args.fr, args.to

    kern_size = (4, 4, 4)
    tensor = np.random.randint(args.fr, args.to, size=(args.batches, args.channels, args.rows, args.cols))
    kern = np.random.randint(args.fr, args.to, size=kern_size)
    
    print(args.batches, args.channels, args.rows, args.cols)
  
    for i in tensor:
      for j in i:
        for k in j:
          for w in k:
            print(w, end=' ')
          print()
        print()
      print()

    for s in kern_size:
      print(s, end=' ')
    print()

    for i in kern:
      for j in i:
        for k in j:
          print(k, end=' ')
        print()
      print()
    
    conv = []
    for img in tensor:
      conv.append(signal.correlate(img, kern, mode='valid'))

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
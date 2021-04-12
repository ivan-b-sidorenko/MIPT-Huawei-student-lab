import numpy as np
from scipy import ndimage

def convolution2d(tensor, kernel, bias):
    m, n = kernel.shape
    if (m == n):
        y, x = tensor.shape
        y = y - m + 1
        x = x - m + 1
        new_image = np.zeros((y,x))
        for i in range(y):
            for j in range(x):
                new_image[i][j] = np.sum(tensor[i:i+m, j:j+m]*kernel) + bias
    return new_image



def main():
    tensor = np.array([[5,5,5,5],
                    [5,5,5,5],
                    [5,5,5,5],
                    [5,5,5,5]])

    kernel = np.array([[2, 2, 2],
                    [2, 2, 2],
                    [2, 2, 2]])

    result = convolution2d(tensor, kernel, 0)

    print(result)


if __name__ == "__main__":
    main()


"""
@file VectorField.py Responsible for parsing data file and plotting the associated vector at each point
"""
import matplotlib.pyplot as plt
import numpy as np
from math import sqrt


I_COOR = []  # I coordinates of points
J_COOR = []  # J coordinates of points
K_COOR = []  # K coordinates of points
X_PLANE = None
Y_PLANE = None
Z_PLANE = None

U = []  # I component of vectors
V = []  # J component of vectors
W = []  # K component of vectors

MAGNITUDES = []

E_FIELD_PATH = "../out/e-field"
B_FIELD_PATH = "../out/b-field"


def import_points(file_path):
    """
    Import points and vector data from a given file
    Each line is expected to be of the form: i j k u v w
    :param file_path: File path containing the vector field data
    :return: None
    """
    global X_PLANE, Y_PLANE, Z_PLANE, MAGNITUDES
    selection = int(input("Enter selection:\n1 for X plane slice\n2 for Y plane slice\n3 for Z plane slice\n4 for all points\n"))

    if selection == 1:
        X_PLANE = float(input("Enter X plane slice value: "))
    elif selection == 2:
        Y_PLANE = float(input("Enter Y plane slice value: "))
    elif selection == 3:
        Z_PLANE = float(input("Enter Z plane slice value: "))

    with open(file_path, 'r') as input_file:
        lines = input_file.readlines()

    for line in lines:
        # line is of the form: i j k u v w
        values = line.split()

        if X_PLANE is None and Y_PLANE is None and Z_PLANE is None:
            I_COOR.append(float(values[0]))
            J_COOR.append(float(values[1]))
            K_COOR.append(float(values[2]))

            U.append(float(values[3]))
            V.append(float(values[4]))
            W.append(float(values[5]))

            MAGNITUDES.append(sqrt((float(values[3]) ** 2) + (float(values[4]) ** 2) + (float(values[5]) ** 2)))

        elif float(values[0]) == X_PLANE:
            J_COOR.append(float(values[1]))
            K_COOR.append(float(values[2]))

            V.append(float(values[4]))
            W.append(float(values[5]))

        elif float(values[1]) == Y_PLANE:
            I_COOR.append(float(values[0]))
            K_COOR.append(float(values[2]))

            U.append(float(values[3]))
            W.append(float(values[5]))

        elif float(values[2]) == Z_PLANE:
            I_COOR.append(float(values[0]))
            J_COOR.append(float(values[1]))

            U.append(float(values[3]))
            V.append(float(values[4]))


def plot():
    """
    Plot all vectors at their respective location
    Note: all vectors are plotted at 2.5 their respective length
    :return: None
    """
    fig = plt.figure()

    if X_PLANE is None and Y_PLANE is None and Z_PLANE is None:
        ax = fig.gca(projection='3d')
        ax.quiver(np.array(I_COOR), np.array(J_COOR), np.array(K_COOR), np.array(U), np.array(V), np.array(W), length=2.5)
    else:
        if X_PLANE is not None:
            ax = plt.axes()

            # Electric Field
            # ax.quiver(np.array(J_COOR), np.array(K_COOR), np.array(V), np.array(W), units="xy", scale_units="dots", scale=None)

            # Magnetic Field
            ax.quiver(np.array(J_COOR), np.array(K_COOR), np.array(V), np.array(W), units="xy", scale_units="dots", scale=None)

            # ax.quiver(np.array(J_COOR), np.array(K_COOR), np.array(V), np.array(W), scale_units="xy", scale=0.1)
            plt.title(f"I slice at i={X_PLANE}")
            plt.xlabel("J coordinates")
            plt.ylabel("K coordinates")
        elif Y_PLANE is not None:
            ax = plt.axes()

            # Electric Field
            ax.quiver(np.array(I_COOR), np.array(K_COOR), np.array(U), np.array(W), units="xy", scale_units="dots", scale=None)

            # Magnetic Field
            # ax.quiver(np.array(I_COOR), np.array(K_COOR), np.array(U), np.array(W), units="xy", scale_units="xy", scale=0.None)

            # Current Field
            ax.quiver(np.array(I_COOR), np.array(K_COOR), np.array(U), np.array(W), units="xy", scale_units="xy", angles="xy", scale=0.000005)

            plt.title(f"J slice at j={Y_PLANE}")
            plt.xlabel("I coordinates")
            plt.ylabel("K coordinates")
        else:
            ax = plt.axes()

            # Electric Field
            # ax.quiver(np.array(I_COOR), np.array(J_COOR), np.array(U), np.array(V), units="xy", scale_units="xy", scale=None)

            # Magnetic Field
            ax.quiver(np.array(I_COOR), np.array(J_COOR), np.array(U), np.array(V), scale_units="xy", scale=0.000008)

            # Current Field
            # ax.quiver(np.array(I_COOR), np.array(J_COOR), np.array(U), np.array(V), scale_units="xy", scale=0.000008)

            plt.title(f"K slice at k={Z_PLANE}")
            plt.xlabel("I coordinates")
            plt.ylabel("J coordinates")

    plt.show()


def configure():
    """
    Prompt the user for the field to plot. Electric & Magnetic Fields are in pre-determined file path.
    Consequently call import_points with the determined file path
    :return: None
    """
    global E_FIELD_PATH, B_FIELD_PATH
    selection = input("Enter selection:\n1: Electric Field\n2: Magnetic Field\n3: Custom file path\n")

    if selection == '1':
        import_points(E_FIELD_PATH)
    elif selection == '2':
        import_points(B_FIELD_PATH)
    elif selection == '3':
        import_points(input("Input custom file path:"))
    else:
        print("Invalid selection, exiting")
        exit(1)


if __name__ == "__main__":
    configure()
    plot()

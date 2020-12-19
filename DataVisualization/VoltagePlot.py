"""
@file VoltagePlot.py Responsible for parsing point coordinates and associated voltage and plotting a heat map
representation of the voltage at each point
"""
import numpy as np
import matplotlib.pyplot as plt
import argparse


I_COOR = []  # I coordinates
J_COOR = []  # J coordinates
K_COOR = []  # K coordinates
VOLTAGES = []  # Voltage corresponding to each each point in parsing order
LOG_FILE_PATH = "../out/initial-voltages"  # Initialize log file path to be normal log file path
X_PLANE = None  # x plane to display associated voltages
Y_PLANE = None  # y plane to display associated voltages
Z_PLANE = None  # z plane to display associated voltages


def parse_arguments():
    """
    Parse command line arguments passed to the program.
    Responsible for collecting the file path of the file path to the initial voltages if provided and any specific plane to plot
    :return: None
    """
    global LOG_FILE_PATH, X_PLANE, Y_PLANE, Z_PLANE

    parser = argparse.ArgumentParser()
    parser.add_argument("--filePath", help="Relative file path to the file containing the initial voltages")
    parser.add_argument("--xPlane", help="Slice in the x plane to display voltages")
    parser.add_argument("--yPlane", help="Slice in the y plane to display voltages")
    parser.add_argument("--zPlane", help="Slice in the z plane to display voltages")

    args = parser.parse_args()

    if args.filePath is not None:
        LOG_FILE_PATH = args.filePath

    X_PLANE = float(args.xPlane) if args.xPlane is not None else None
    Y_PLANE = float(args.yPlane) if args.yPlane is not None else None
    Z_PLANE = float(args.zPlane) if args.zPlane is not None else None


def import_points(file_path):
    """
    Import all points and it's corresponding value
    Each line is expected to be of the form: i j k value
    :param file_path: File path of the file containing the data to plot
    :return: None
    """
    with open(file_path, 'r') as csv:
        input_file = csv.readlines()

    for line in input_file:
        # lines are of the form: i j k voltage
        values = line.split()

        if X_PLANE is None and Y_PLANE is None and Z_PLANE is None:  # no specific plane specified, plot all voltages
            I_COOR.append(float(values[0]))
            J_COOR.append(float(values[1]))
            K_COOR.append(float(values[2]))
            VOLTAGES.append(float(values[4]))

        if float(values[0]) == X_PLANE or float(values[1]) == Y_PLANE or float(values[2]) == Z_PLANE:
            # specific plane specified
            I_COOR.append(float(values[0]))
            J_COOR.append(float(values[1]))
            K_COOR.append(float(values[2]))
            VOLTAGES.append(float(values[4]))


def plot():
    """
    Plot the specific plane or all points
    :return: None
    """
    fig = plt.figure()
    if X_PLANE is None and Y_PLANE is None and Z_PLANE is None:  # display all points
        ax = plt.axes(projection="3d")

        data_plot = ax.scatter3D(np.array(I_COOR), np.array(J_COOR), np.array(K_COOR), c=np.array(VOLTAGES))
        fig.colorbar(data_plot)
    else:
        if X_PLANE is not None:  # display slice from x plane
            data_plot = plt.scatter(np.array(J_COOR), np.array(K_COOR), c=np.array(VOLTAGES))
            plt.title(f"I slice at i={X_PLANE}")
            plt.xlabel("J coordinates")
            plt.ylabel("K coordinates")
            fig.colorbar(data_plot)
        elif Y_PLANE is not None:  # display slice from y plane
            data_plot = plt.scatter(np.array(I_COOR), np.array(K_COOR), c=np.array(VOLTAGES))
            plt.title(f"J slice at j={Y_PLANE}")
            plt.xlabel("I coordinates")
            plt.ylabel("K coordinates")
            fig.colorbar(data_plot)
        else:  # display slice from z plane
            data_plot = plt.scatter(np.array(I_COOR), np.array(J_COOR), c=np.array(VOLTAGES))
            plt.title(f"K slice at k={Z_PLANE}")
            plt.xlabel("I coordinates")
            plt.ylabel("J coordinates")
            fig.colorbar(data_plot)
    plt.show()


if __name__ == "__main__":
    parse_arguments()  # parse command line arguments
    import_points(LOG_FILE_PATH)  # import needed points
    plot()  # plot specified plane or 3d plot of all voltages

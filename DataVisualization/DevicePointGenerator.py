from PIL import Image
import numpy as np

IMG_DATA = None
IMG_ARRAY = None


def import_image(file_path):
    global IMG_DATA, IMG_ARRAY
    IMG_DATA = Image.open(file_path)
    IMG_DATA.load()

    IMG_ARRAY = np.asarray(IMG_DATA)


def save_device_points_to_file(file_path):
    global IMG_DATA, IMG_ARRAY
    indices = np.argwhere(IMG_ARRAY <= 150)

    generated_pts = []

    with open(file_path, 'w') as output_file:
        for ind in indices:
            pt = (ind[0], ind[1])
            if pt not in generated_pts:
                generated_pts.append(pt)
                output_file.write(f"{ind[0]} {ind[1]} \n")


if __name__ == "__main__":
    # import_image(input("Enter device image path: "))
    # save_device_points_to_file(input("Enter file path to save device points: "))
    import_image("test-image-6.png")
    save_device_points_to_file("pts.txt")

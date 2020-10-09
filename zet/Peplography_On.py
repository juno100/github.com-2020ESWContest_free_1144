import cv2
import numpy as np
import pyzed.sl as sl
import matplotlib.pyplot as plt


def main(ft_in_img=None, outt_img=None):
    print("Running...")
    init = sl.InitParameters()
    init.camera_resolution = sl.RESOLUTION.VGA

    cam = sl.Camera()
    if not cam.is_opened():
        print("Opening ZED Camera...")
    status = cam.open(init)
    if status != sl.ERROR_CODE.SUCCESS:
        print(repr(status))
        exit()

    runtime = sl.RuntimeParameters()
    runtime.sensing_mode = sl.SENSING_MODE.FILL

    image_size = cam.get_camera_information().camera_resolution
    width = image_size.width
    height = image_size.height

    mat_l = sl.Mat(width, height, sl.MAT_TYPE.U8_C4)
    mat_r = sl.Mat(width, height, sl.MAT_TYPE.U8_C4)

    key = ''

    kx = 0.5
    ky = 0.5
    max_f = 3

    clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))

    while key != 113:  # for 'q' key
        err = cam.grab(runtime)
        if err == sl.ERROR_CODE.SUCCESS:
            cam.retrieve_image(mat_l, sl.VIEW.LEFT_GRAY)
            cam.retrieve_image(mat_r, sl.VIEW.RIGHT)

            right_image = mat_r.get_data()
            left_image = mat_l.get_data()

            right_image = right_image[..., :3]

            v = np.shape(right_image)[0]
            h = np.shape(right_image)[1]
            d = np.shape(right_image)[2]

            f_in_img = np.zeros(np.shape(right_image), dtype=np.complex_)

            f_in_img[:, :, 0] = np.fft.fft2(right_image[:, :, 0])
            f_in_img[:, :, 1] = np.fft.fft2(right_image[:, :, 1])
            f_in_img[:, :, 2] = np.fft.fft2(right_image[:, :, 2])


            f_in_img[0, 0:int(np.round(kx * (h - 1))), :] = f_in_img[0, 0:int(np.round(kx * (h - 1))), :] / max_f
            f_in_img[0:int(np.round(ky * (v - 1))), 0, :] = f_in_img[0:int(np.round(ky * (v - 1))), 0, :] / max_f

            f_in_img[0, int(np.round((0 - kx) * (h - 1))): h - 1, :] = f_in_img[0,int(np.round((0 - kx) * (h - 1))): h - 1, :] / max_f  # check(0,o)
            f_in_img[0:int(np.round(ky * (v - 1))), h - 1, :] = f_in_img[0:int(np.round(ky * (v - 1))), h - 1,:] / max_f

            f_in_img[int(np.round((0 - ky) * (v - 1))): v - 1, 0, :] = f_in_img[ int(np.round((0 - ky) * (v - 1))): v - 1, 0,:] / max_f
            f_in_img[v - 1, 0:int(np.round(kx * (h - 1))), :] = f_in_img[v - 1, 0:int(np.round(kx * (h - 1))),:] / max_f

            f_in_img[int(np.round((0 - ky) * (v - 1))): v - 1, h - 1, :] = f_in_img[int(np.round((0 - ky) * (v - 1))): v - 1, h - 1, :] / max_f
            f_in_img[v - 1, int(np.round((0 - kx) * (h - 1))): h - 1, :] = f_in_img[v - 1, int(np.round((0 - kx) * (h - 1))): h - 1,:] / max_f

            out_img = np.zeros(np.shape(f_in_img), dtype=np.float_)

            out_img[:, :, 0] = abs(np.fft.ifft2(f_in_img[:, :, 0]))
            out_img[:, :, 1] = abs(np.fft.ifft2(f_in_img[:, :, 1]))
            out_img[:, :, 2] = abs(np.fft.ifft2(f_in_img[:, :, 2]))

            out_img = out_img / np.max(out_img[:])

            cv2.imshow("Fourier Peplography", out_img)

            cv2.imshow("RIGHT_IMAGE", right_image)
            cv2.imshow("LEFT_IMAGE", left_image)
            key = cv2.waitKey(5)

        else:
            key = cv2.waitKey(5)
    cv2.destroyAllWindows()
    cam.close()
    print("\nFINISH")


if __name__ == "__main__":
    main()

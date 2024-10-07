#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/utility.hpp>
#include <algorithm>
using namespace std;

cv::Mat readingImage(const string& path, bool color = false, bool display = false) {
    cv::Mat image;
    if (color) {
        image = cv::imread(path, cv::IMREAD_COLOR);
    }
    else {
        image = cv::imread(path, cv::IMREAD_GRAYSCALE);
    }
    if (display) {
        cv::imshow("Display image",image);
        cv::waitKey(0);
    }
    return image;
}

void combine(const string& path1, const string& path2, const bool color, const bool display) {
    cv:: Mat image1 = readingImage(path1, color, display);
    cv:: Mat image2 = readingImage(path2, color, display);
    cv:: Mat result = image1.clone();

    cv::resize(result, result, cv::Size(image2.cols, image2.rows));

    for (int i = 0; i < result.rows; i++) {
        for (int j = 0; j < result.cols; j++) {
            cv::Vec3b pixelValue1 = result.at<cv::Vec3b>(i, j);
            cv::Vec3b pixelValue2 = image2.at<cv::Vec3b>(i, j);
            for (int c = 0; c < 3; c++) {
                pixelValue1[c] = static_cast<uchar>((static_cast<int>(pixelValue1[c]) + static_cast<int>(pixelValue2[c])) / 2);
            }
            result.at<cv::Vec3b>(i, j) = pixelValue1;
        }
    }

    cv::imshow("Combined imaged",result);
    cv::waitKey(0);
}

cv::Mat image_noise(string& path) {
    cv::Mat image = readingImage(path, false, false);
    cv::Mat gaussian_noise(image.size(), image.type());

    double mean = 2;
    double stddev = 20.0;

    cv::randn(gaussian_noise, mean, stddev);

    cv::Mat noisy_image;
    cv::add(image, gaussian_noise, noisy_image);

    cv::Mat resized_image;
    cv::resize(noisy_image, resized_image, cv::Size(), 2.0, 2.0);

    cv::imshow("Noisy Image", resized_image);
    cv::waitKey(0);

    string save_path = "Images/noisy.jpg";
    cv::imwrite(save_path, noisy_image);

    return resized_image;
}

cv::Mat median_blur(const cv::Mat& image, int kernel_size = 3) {
    if (kernel_size % 2 == 0) {
        kernel_size += 1;
    }

    int pad_size = kernel_size / 2;
    cv::Mat output = cv::Mat::zeros(image.size(), image.type());

    cv::Mat padded_image;
    cv::copyMakeBorder(image, padded_image, pad_size, pad_size, pad_size, pad_size, cv::BORDER_CONSTANT, 0);

    for (int i = pad_size; i < image.rows + pad_size; i++) {
        for (int j = pad_size; j < image.cols + pad_size; j++) {
            std::vector<uchar> neighborhood;

            for (int k = -pad_size; k <= pad_size; k++) {
                for (int l = -pad_size; l <= pad_size; l++) {
                    neighborhood.push_back(padded_image.at<uchar>(i + k, j + l));
                }
            }

            ranges::sort(neighborhood);
            output.at<uchar>(i - pad_size, j - pad_size) = neighborhood[neighborhood.size() / 2];
        }
    }

    return output;
}

cv::Mat exchange_color_image(cv::Mat img, const cv::Vec3i& C) {
    int h = img.rows;
    int w = img.cols;
    int c = img.channels();

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            for (int k = 0; k < c; k++) {
                uchar pixel = img.at<cv::Vec3b>(i, j)[k];
                if (static_cast<int>(pixel) + C[k] > 255) {
                    img.at<cv::Vec3b>(i, j)[k] = 255;
                } else if (static_cast<int>(pixel) + C[k] < 0) {
                    img.at<cv::Vec3b>(i, j)[k] = 0;
                } else {
                    img.at<cv::Vec3b>(i, j)[k] += C[k];
                }
            }
        }
    }
    return img;
}

cv::Mat exchange_gray_image(cv::Mat img, int c) {
    int h = img.rows;
    int w = img.cols;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            uchar pixel = img.at<uchar>(i, j);
            if (static_cast<int>(pixel) + c >= 255) {
                img.at<uchar>(i, j) = 255;
            } else if (static_cast<int>(pixel) + c < 0) {
                img.at<uchar>(i, j) = 0;
            } else {
                img.at<uchar>(i, j) += c;
            }
        }
    }

    cout << static_cast<int>(img.at<uchar>(0, 0)) << endl;
    cout << static_cast<int>(img.at<uchar>(100, 100)) << endl;
    cout << static_cast<int>(img.at<uchar>(190, 140)) << endl;

    return img;
}

void histogram(string path) {
    cv::Mat img = imread(path, cv::IMREAD_GRAYSCALE);
    cv::Mat img_equalized;
    cv::equalizeHist(img, img_equalized);

    cv::imshow("Original Image", img);
    cv::imshow("Equalized Image", img_equalized);
    cv::waitKey(0);
}

int main() {
    string path = "Images/gray_image/image2.jpg";
    cv::Mat image = readingImage(path, true, false);
    cout << "Image size: " << image.size() << endl;
    string path1 = "Images/color_image/image3.jpg";
    string path2 = "Images/color_image/image4.jpg";
    // combine(path1, path2, true, false);

    string path3 = "Images/gray_image/images.jpg";
    // cv:: Mat noisy_image = image_noise(path3);

    // cv::Mat filtered_image = median_blur(noisy_image);
    // imshow("Filtered image", filtered_image);
    // cv::waitKey(0);

    cv::Mat gray_image = exchange_gray_image(image, 255);
    // cv::Mat resized;
    // cv::resize(color_image,resized,cv::Size(color_image.cols , color_image.rows * 4));
    // imshow("Gray Image", resized);
    // cv::waitKey(0);

    // int b = 0, g = 0, r = 100;
    // cv::Vec3i C = {b, g, r};
    // cv::Mat color_image = exchange_color_image(readingImage(path1, true), C);
    // imshow("Color", color_image);
    // cv::waitKey(0);

    histogram(path1);

    return 0;

}

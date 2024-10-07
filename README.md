# Báo cáo sử dụng OpenCV và Đa luồng

> Báo cáo cách sử dụng thư viện OpenCV và Threads trong C++.

Tóm tắt các kiến thức tìm hiểu và thực hành:
* Cách đọc (Ảnh màu, ảnh xám) và lưu ảnh
* Kích thước ảnh, kênh ảnh
* Cách lấy và thay đổi giá trị điểm ảnh
* Tăng cường độ sáng (Theo kênh màu/giá trị)
* Thay đổi kích thước ảnh
* Tạo nhiễu Gaussian
* Bộ lọc Median Blur
* Cân bằng histogram

## Đọc và lưu ảnh

Cú pháp:

* **Đọc ảnh**
```c++
cv::Mat image = cv::imread("path/to/your/image.jpg", cv::IMREAD_COLOR);
```
Trong đó `cv::IMREAD_COLOR` là flag tương ứng với "cách đọc" ảnh khác nhau như xám/ảnh màu.

* **Lưu ảnh**
```c++
cv::imwrite("path/to/your/save/image.jpg", image);
```
Trong đó `image` là ảnh cần lưu.

## Kích thước ảnh và kênh màu

### Các cách lấy kích thước ảnh
* Sử dụng `size()`
```c++
cv::Size imageSize = image.size();
```
* Sử dụng `rows` và `cols`
```c++
int h = image.rows;
int w = image.cols;
```

### Lấy kênh màu
```c++
int channels = image.channels();
```

## Cách lấy và thay đổi giá trị điểm ảnh
Để lấy giá trị điểm ảnh tại vị trí `(x, y)`:
```c++
cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
```
Để thay đổi giá trị điểm ảnh:
```c++
image.at<cv::Vec3b>(y, x) = cv::Vec3b(255, 0, 0); // Đặt điểm ảnh thành màu đỏ
```

## Tăng cường độ sáng
Để tăng cường độ sáng của ảnh, có thể cộng một giá trị cố định cho mỗi kênh màu:
```c++
for (int y = 0; y < image.rows; y++) {
    for (int x = 0; x < image.cols; x++) {
        cv::Vec3b& pixel = image.at<cv::Vec3b>(y, x);
        for (int c = 0; c < 3; c++) {
            pixel[c] = cv::saturate_cast<uchar>(pixel[c] + 50); // Tăng sáng 50
        }
    }
}
```

## Thay đổi kích thước ảnh
Sử dụng hàm `resize` để thay đổi kích thước ảnh:
```c++
cv::Mat resizedImage;
cv::resize(image, resizedImage, cv::Size(newWidth, newHeight));
```

## Tạo nhiễu Gaussian
Sử dụng hàm `randn` để tạo nhiễu Gaussian:
```c++
cv::Mat noise(image.size(), image.type());
cv::randn(noise, 0, 50); // Trung bình = 0, độ lệch chuẩn = 50
cv::Mat noisyImage;
cv::add(image, noise, noisyImage);
```

## Bộ lọc Median Blur
Sử dụng hàm `medianBlur` để làm mờ ảnh:
```c++
cv::Mat blurredImage;
cv::medianBlur(image, blurredImage, 5); // Kích thước kernel = 5
```

## Cân bằng histogram
Để cân bằng histogram cho ảnh màu:
```c++
std::vector<cv::Mat> channels;
cv::split(image, channels);
for (int i = 0; i < channels.size(); i++) {
    cv::equalizeHist(channels[i], channels[i]);
}
cv::merge(channels, image);
```
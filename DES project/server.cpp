#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080  // Cổng server lắng nghe
#define BUFFER_SIZE 1024  // Kích thước buffer

void receiveFile(const std::string& outputFilename) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];

    // 1️⃣ Tạo socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "❌ Lỗi tạo socket!" << std::endl;
        return;
    }

    // 2️⃣ Thiết lập địa chỉ server
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 3️⃣ Bind socket vào địa chỉ và cổng
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "❌ Lỗi bind!" << std::endl;
        return;
    }

    // 4️⃣ Lắng nghe kết nối
    if (listen(server_fd, 3) < 0) {
        std::cerr << "❌ Lỗi listen!" << std::endl;
        return;
    }
    std::cout << "📡 Server đang lắng nghe trên cổng " << PORT << "..." << std::endl;

    // 5️⃣ Chấp nhận kết nối từ Client
    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);
    if (new_socket < 0) {
        std::cerr << "❌ Lỗi accept!" << std::endl;
        return;
    }
    std::cout << "✅ Kết nối nhận file thành công!" << std::endl;

    // 6️⃣ Nhận dữ liệu và ghi vào file
    std::ofstream outFile(outputFilename, std::ios::binary);
    if (!outFile) {
        std::cerr << "❌ Không thể mở file để ghi!" << std::endl;
        return;
    }

    int bytesRead;
    while ((bytesRead = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
        outFile.write(buffer, bytesRead);
    }

    std::cout << "✅ Đã nhận file và lưu vào: " << outputFilename << std::endl;

    // 7️⃣ Đóng kết nối
    close(new_socket);
    close(server_fd);
}

int main() {
    receiveFile("received_ciphertext.txt");
    return 0;
}

# 🌟 Gold Miner!

### Author: Hoàng Sỹ Hùng
### Student ID: 24022657
### University: University of Engineering and Technology, Vietnam National University Hanoi

# 📃 Description

*Gold Miner!* is a simple 2D arcade game written in C++ using the SDL2 library. Players control a miner using a claw to collect gold, gems, and other treasures within a time limit while avoiding obstacles like rocks.

## Key Features:
- **Controls:** A/D or left/right arrow keys (move claw), Space (launch claw).
- **Game Area:** 800x600 pixel game window with a main play area containing treasures and obstacles.
- **Scoring System:** Points based on the value and weight of collected treasures.
- **Time Limit:** Countdown timer to meet score objectives.
- **Collision Handling:** Claw interacts with treasures and obstacles.
- **Simple Menu:** Includes start, audio settings, and quit options.

## Technologies:
- **Language:** C++.
- **Libraries:** SDL2, SDL2_image, SDL2_ttf, SDL2_mixer.

# ❗ Notes

Since the game was developed and tested using Code::Blocks, it performs best on Code::Blocks. If using Visual Studio Code, please set up SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer to run the code.

# 🛠 System Requirements

- **Operating System:** Windows, Linux, or macOS (tested on Windows).
- **Compiler:** G++ or any C++ compiler supporting C++11 or later.
- **Required Libraries:**
  - SDL2
  - SDL2_image
  - SDL2_ttf
  - SDL2_mixer
- **Additional Requirements for Windows with Code::Blocks:**
  - Install SDL2, SDL2_image, SDL2_ttf, and SDL2_mixer.
  - Copy SDL2.dll, SDL2_image.dll, SDL2_ttf.dll, and SDL2_mixer.dll to the directory containing the source code (where the .cbp file or executable is located) to run the game.

# 🎮 How to Play

**Objective:** Collect enough points from gold, gems, and other treasures to complete the level before time runs out.

## Controls:
- **A or Left Arrow:** Move claw left.
- **D or Right Arrow:** Move claw right.
- **Space:** Launch claw to collect treasures.
- **ESC:** Pause the game.
- **Lose:** If you fail to meet the score goal before time runs out, press any key to retry.
- **Win:** Reach the required score to complete the level.

# 🏆 Credits

- **Game Concept & Programming:** Hoàng Sỹ Hùng
- **SDL2 Library References:**
  - [Lazyfoo SDL Tutorials](https://lazyfoo.net/tutorials/SDL/index.php)
  - [Tianchinchiko YouTube Channel](https://www.youtube.com/@tianchinchiko)
  - Advanced Programming course materials
- **Graphics:**
  - [OpenGameArt](https://opengameart.org)
  - Self-designed using [LibreSprite](https://github.com/LibreSprite/libresprite.github.io/blob/master/install.md)
- **Audio:**
  - Background Music: [Pixabay](https://pixabay.com)
  - Sound Effects: Retro SFX collections
- **Inspiration:**
  - Classic Gold Miner games
  - [Phaser Game Framework](https://phaser.io/examples/v3.85.0)

---

# 🌟 Đào Vàng!

### Tác giả: Hoàng Sỹ Hùng
### Mã sinh viên: 24022657
### Trường đại học: Trường Đại học Công Nghệ - Đại học Quốc gia Hà Nội

# 📃 Mô tả

*Đào Vàng!* là một trò chơi arcade 2D đơn giản được viết bằng C++ sử dụng thư viện SDL2. Người chơi điều khiển một thợ mỏ sử dụng móc kéo để thu thập vàng, đá quý và các kho báu khác trong thời gian giới hạn, đồng thời tránh các chướng ngại vật như đá.

## Tính năng chính:
- **Điều khiển:** A/D hoặc mũi tên trái/phải (di chuyển móc), Space (thả móc).
- **Vùng hoạt động:** Màn hình trò chơi 800x600 pixel với khu vực chơi chính chứa kho báu và chướng ngại vật.
- **Hệ thống điểm:** Điểm số dựa trên giá trị và trọng lượng kho báu.
- **Thời gian giới hạn:** Đồng hồ đếm ngược để hoàn thành mục tiêu điểm.
- **Xử lý va chạm:** Móc kéo tương tác với kho báu và chướng ngại vật.
- **Menu đơn giản:** Bao gồm tùy chọn bắt đầu, cài đặt âm thanh, và thoát.

## Công nghệ:
- **Ngôn ngữ:** C++.
- **Thư viện:** SDL2, SDL2_image, SDL2_ttf, SDL2_mixer.

# ❗ Lưu ý

Vì game được phát triển và kiểm tra bằng Code::Blocks, game hoạt động tốt nhất trên Code::Blocks. Nếu sử dụng Visual Studio Code, hãy tự thiết lập SDL2, SDL2_image, SDL2_ttf, và SDL2_mixer để chạy mã nguồn.

# 🛠 Yêu cầu hệ thống

- **Hệ điều hành:** Windows, Linux, hoặc macOS (đã kiểm tra trên Windows).
- **Công cụ biên dịch:** G++ hoặc bất kỳ trình biên dịch C++ nào hỗ trợ C++11 trở lên.
- **Thư viện cần thiết:**
  - SDL2
  - SDL2_image
  - SDL2_ttf
  - SDL2_mixer
- **Yêu cầu bổ sung cho Windows nếu dùng Code::Blocks:**
  - Cần cài đặt SDL2, SDL2_image, SDL2_ttf, và SDL2_mixer.
  - Sao chép các file SDL2.dll, SDL2_image.dll, SDL2_ttf.dll, và SDL2_mixer.dll vào thư mục chứa mã nguồn (nơi có file .cbp hoặc file thực thi sau khi build) để chạy trò chơi.

# 🎮 Cách chơi

**Mục tiêu:** Thu thập đủ điểm từ vàng, đá quý và các kho báu khác để hoàn thành cấp độ trước khi hết thời gian.

## Điều khiển:
- **A hoặc Mũi tên trái:** Di chuyển móc sang trái.
- **D hoặc Mũi tên phải:** Di chuyển móc sang phải.
- **Space:** Thả móc để thu thập kho báu.
- **ESC:** Tạm dừng trò chơi.
- **Thua:** Nếu không đạt mục tiêu điểm trước khi hết giờ, nhấn phím bất kỳ để chơi lại.
- **Thắng:** Đạt đủ điểm để hoàn thành cấp độ.

# 🏆 Ghi công

- **Khái niệm trò chơi & Lập trình:** Hoàng Sỹ Hùng
- **Tham khảo thư viện SDL2:**
  - [Hướng dẫn SDL của Lazyfoo](https://lazyfoo.net/tutorials/SDL/index.php)
  - [Kênh YouTube Tianchinchiko](https://www.youtube.com/@tianchinchiko)
  - Tài liệu lớp từ Lập trình nâng cao
- **Đồ họa:**
  - [OpenGameArt](https://opengameart.org)
  - Tự thiết kế sử dụng [LibreSprite](https://github.com/LibreSprite/libresprite.github.io/blob/master/install.md)
- **Âm thanh:**
  - Nhạc nền: [Pixabay](https://pixabay.com)
  - Hiệu ứng âm thanh: Bộ sưu tập SFX retro
- **Nguồn cảm hứng:**
  - Game Đào Vàng kinh điển
  - [Phaser Game Framework](https://phaser.io/examples/v3.85.0)
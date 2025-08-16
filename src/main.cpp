
// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <Fonts/FreeMonoBold9pt7b.h>

// --- Định nghĩa các chân kết nối ---
// Các chân điều khiển
#define CS_PIN    5
#define BUSY_PIN  4
#define RES_PIN   17
#define DC_PIN    16

#define SCK_PIN   12  // Chân SCL (Clock)
#define MOSI_PIN  11  // Chân SDA (Data)

// Chọn driver cho màn hình của bạn (ví dụ: màn 4.2 inch màu)
GxEPD2_3C<GxEPD2_420c_GDEY042Z98, GxEPD2_420c_GDEY042Z98::HEIGHT> display(GxEPD2_420c_GDEY042Z98(/*CS=5*/ CS_PIN, /*DC=*/ DC_PIN, /*RES=*/ RES_PIN, /*BUSY=*/ BUSY_PIN));

const char HelloWorld[] = "Hello World!";
const char HelloWeACtStudio[] = "WeAct Studio";

// Hàm kiểm tra kết nối e-ink
void kiemTraChanEink() {
  Serial.println("=== Kiem tra chan ket noi voi E-Ink ===");

  // 1. Test BUSY (nếu không kéo xuống/up thì có vấn đề)
  pinMode(BUSY_PIN, INPUT);
  int busyState = digitalRead(BUSY_PIN);
  Serial.print("BUSY_PIN (");
  Serial.print(BUSY_PIN);
  Serial.print(") = ");
  Serial.println(busyState == HIGH ? "HIGH" : "LOW");

  // 2. Test RST: cho reset thủ công và đọc BUSY
  pinMode(RES_PIN, OUTPUT);
  digitalWrite(RES_PIN, LOW);
  delay(100);
  digitalWrite(RES_PIN, HIGH);
  delay(100);
  Serial.println("Da gui tin hieu RESET");

  // 3. Thử khởi tạo màn hình -> nếu SPI + CS + DC đúng thì BUSY sẽ thay đổi
  Serial.println("Dang khoi tao man hinh...");
  display.init(115200);

  // 4. Thử vẽ một ô vuông nhỏ để xem BUSY có hoạt động
  display.setPartialWindow(0, 0, 50, 50);
  display.firstPage();
  do {
    display.fillScreen(GxEPD_BLACK);
  } while (display.nextPage());
  Serial.println("Ve o vuong test xong!");

  // 5. Đọc lại BUSY sau refresh
  busyState = digitalRead(BUSY_PIN);
  Serial.print("BUSY_PIN sau khi refresh = ");
  Serial.println(busyState == HIGH ? "HIGH" : "LOW");

  Serial.println("=== Kiem tra ket noi hoan tat ===");
}

void helloWorld()
{
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center the bounding box by transposition of the origin:
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  display.setFullWindow();
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y-tbh);
    display.print(HelloWorld);
    display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
    display.getTextBounds(HelloWeACtStudio, 0, 0, &tbx, &tby, &tbw, &tbh);
    x = ((display.width() - tbw) / 2) - tbx;
    display.setCursor(x, y+tbh);
    display.print(HelloWeACtStudio);
  }
  while (display.nextPage());
}

void helloFullScreenPartialMode()
{
  //Serial.println("helloFullScreenPartialMode");
  const char fullscreen[] = "full screen update";
  const char fpm[] = "fast partial mode";
  const char spm[] = "slow partial mode";
  const char npm[] = "no partial mode";
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  if (display.epd2.WIDTH < 104) display.setFont(0);
  display.setTextColor(GxEPD_BLACK);
  const char* updatemode;
  if (display.epd2.hasFastPartialUpdate)
  {
    updatemode = fpm;
  }
  else if (display.epd2.hasPartialUpdate)
  {
    updatemode = spm;
  }
  else
  {
    updatemode = npm;
  }
  // do this outside of the loop
  int16_t tbx, tby; uint16_t tbw, tbh;
  // center update text
  display.getTextBounds(fullscreen, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t utx = ((display.width() - tbw) / 2) - tbx;
  uint16_t uty = ((display.height() / 4) - tbh / 2) - tby;
  // center update mode
  display.getTextBounds(updatemode, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t umx = ((display.width() - tbw) / 2) - tbx;
  uint16_t umy = ((display.height() * 3 / 4) - tbh / 2) - tby;
  // center HelloWorld
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t hwx = ((display.width() - tbw) / 2) - tbx;
  uint16_t hwy = ((display.height() - tbh) / 2) - tby;
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(hwx, hwy);
    display.print(HelloWorld);
    display.setCursor(utx, uty);
    display.print(fullscreen);
    display.setCursor(umx, umy);
    display.print(updatemode);
  }
  while (display.nextPage());
  //Serial.println("helloFullScreenPartialMode done");
}

void showPartialUpdate()
{
  // some useful background
  helloWorld();
  // use asymmetric values for test
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 70;
  uint16_t box_h = 20;
  uint16_t cursor_y = box_y + box_h - 6;
  if (display.epd2.WIDTH < 104) cursor_y = box_y + 6;
  float value = 13.95;
  uint16_t incr = display.epd2.hasFastPartialUpdate ? 1 : 3;
  display.setFont(&FreeMonoBold9pt7b);
  if (display.epd2.WIDTH < 104) display.setFont(0);
  display.setTextColor(GxEPD_BLACK);
  // show where the update box is
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    display.setPartialWindow(box_x, box_y, box_w, box_h);
    display.firstPage();
    do
    {
      display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
      //display.fillScreen(GxEPD_BLACK);
    }
    while (display.nextPage());
    delay(2000);
    display.firstPage();
    do
    {
      display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    }
    while (display.nextPage());
    delay(1000);
  }
  //return;
  // show updates in the update box
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    display.setPartialWindow(box_x, box_y, box_w, box_h);
    for (uint16_t i = 1; i <= 10; i += incr)
    {
      display.firstPage();
      do
      {
        display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
        display.setCursor(box_x, cursor_y);
        display.print(value * i, 2);
      }
      while (display.nextPage());
      delay(500);
    }
    delay(1000);
    display.firstPage();
    do
    {
      display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    }
    while (display.nextPage());
    delay(1000);
  }
}

// void setup()
// {
//   display.init(115200,true,50,false);
//   helloWorld();
//   helloFullScreenPartialMode();
//   delay(1000);
//   if (display.epd2.hasFastPartialUpdate)
//   {
//     showPartialUpdate();
//     delay(1000);
//   }
//   display.hibernate();
// }

const char HelloWeActStudio[] = "WeAct Studio";

void setup() {
  Serial.begin(115200);
  Serial.println("Khoi dong E-Ink mau...");

  SPI.begin(SCK_PIN, -1, MOSI_PIN);

  display.init(115200);
  display.setRotation(0);
  display.setFont(&FreeMonoBold9pt7b);

  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);

    // Vẽ border dày 5px (đen)
    display.fillRect(0, 0, display.width(), 5, GxEPD_BLACK);                        
    display.fillRect(0, display.height()-5, display.width(), 5, GxEPD_BLACK);       
    display.fillRect(0, 0, 5, display.height(), GxEPD_BLACK);                       
    display.fillRect(display.width()-5, 0, 5, display.height(), GxEPD_BLACK);       

    // --- In chữ Hello (màu đỏ, căn giữa) ---
    int16_t tbx, tby; 
    uint16_t tbw, tbh;
    display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
    int16_t x1 = (display.width() - tbw) / 2 - tbx;
    int16_t y1 = (display.height() / 2) - tbh;  
    display.setTextColor(GxEPD_RED);
    display.setCursor(x1, y1);
    display.print(HelloWorld);

    // --- In chữ WeAct Studio (đen, căn giữa) ---
    display.getTextBounds(HelloWeACtStudio, 0, 0, &tbx, &tby, &tbw, &tbh);
    int16_t x2 = (display.width() - tbw) / 2 - tbx;
    int16_t y2 = (display.height() / 2) + tbh + 10;
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(x2, y2);
    display.print(HelloWeACtStudio);

  } while (display.nextPage());

  Serial.println("Xong!");
}

void loop() {
  // để trống
}

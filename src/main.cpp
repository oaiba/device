
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

GxEPD2_3C<GxEPD2_420c_GDEY042Z98, GxEPD2_420c_GDEY042Z98::HEIGHT> display(GxEPD2_420c_GDEY042Z98(/*CS=5*/ CS_PIN, /*DC=*/ DC_PIN, /*RES=*/ RES_PIN, /*BUSY=*/ BUSY_PIN));

const char HelloWorld[] = "Hello World!";
const char HelloWeACtStudio[] = "WeAct Studio";

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
  display.hibernate();
}

void loop() {
  // Không làm gì trong vòng lặp chính
  // Chế độ Hibernate
}

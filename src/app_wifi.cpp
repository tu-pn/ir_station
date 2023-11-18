
#include "app_wifi.h"
#include "WiFi.h"

#define WIFI_DEFAULT_SSID_SIZE 32 // refer to arduino WiFi.h
#define WIFI_DEFAULT_PASS_SIZE 32

const char *WIFI_DEFAULT_SSID = "Benkon";
const char *WIFI_DEFAULT_PASS = "BenKon@123";

// STAIC function declaration

static wifi_state_t parseInput();
static void usage();
static uint8_t getCli(char *data, uint8_t len);
static void wifiScan();
static void wifiConnectCLI();
static bool wifiConnect(const char *ssid, const char *password);


// END static func

static void wifiScan()
{
    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    // WiFi.scanNetworks will return the number of networks found.
    int n = WiFi.scanNetworks();
    Serial.println("Scan done");
    if (n == 0)
    {
        Serial.println("no networks found");
    }
    else
    {
        Serial.print(n);
        Serial.println("Networks found");
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            Serial.printf("%2d", i + 1);
            Serial.print(" | ");
            Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
            Serial.print(" | ");
            Serial.printf("%4d", WiFi.RSSI(i));
            Serial.print(" | ");
            Serial.printf("%2d", WiFi.channel(i));
            Serial.print(" | ");
            switch (WiFi.encryptionType(i))
            {
            case WIFI_AUTH_OPEN:
                Serial.print("open");
                break;
            case WIFI_AUTH_WEP:
                Serial.print("WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                Serial.print("WPA");
                break;
            case WIFI_AUTH_WPA2_PSK:
                Serial.print("WPA2");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                Serial.print("WPA+WPA2");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                Serial.print("WPA2-EAP");
                break;
            case WIFI_AUTH_WPA3_PSK:
                Serial.print("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                Serial.print("WPA2+WPA3");
                break;
            case WIFI_AUTH_WAPI_PSK:
                Serial.print("WAPI");
                break;
            default:
                Serial.print("unknown");
            }
            Serial.println();
            delay(10);
        }
    }
    Serial.println("");

    // Delete the scan result to free memory for code below.
    WiFi.scanDelete();
}

static bool wifiConnect(const char *ssid, const char *password)
{
    WiFi.begin(ssid, password);

    delay(1000);
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Connecting to WiFi...");
    }

    return wifiIsConnected();
}

bool wifiIsConnected()
{
    switch (WiFi.status())
    {
    case WL_NO_SSID_AVAIL:
        Serial.printf("%s is unreachable\n", WiFi.SSID());
        return false;

    case WL_CONNECTED:
        Serial.printf("Connected to wifi %", WiFi.SSID());
        return true;

    default:
        Serial.printf("Connect to %s error: %u\n", WiFi.SSID(), WiFi.status());
        break;
    }

    return false;
}

static void wifiConnectCLI()
{

    char ssid[WIFI_DEFAULT_SSID_SIZE] = {0};
    char pass[WIFI_DEFAULT_PASS_SIZE] = {0};

    Serial.println("Usage:");
    Serial.printf (" Wifi name/pass should not greater than %u characters\n", WIFI_DEFAULT_SSID_SIZE);

    Serial.printf("Wifi Name: ");
    getCli(ssid, WIFI_DEFAULT_SSID_SIZE - 1);

    Serial.printf("Wifi Pass: ");
    getCli(pass, WIFI_DEFAULT_PASS_SIZE - 1);

    wifiConnect(ssid, pass);
}

static void usage()
{
    char *usage_str = "Usage:\n"
                      "   [-h] [--foo]\n"
                      " options:\n"
                      "  -h        Show this help message"
                      "  -s        Scan all available ssid\n"
                      "  -c        Connect to ssid\n"
                      "  -d        Connect to default wifi ssid=Benkon, pass=Benkon@123\n"
                      "  -e        Exit wifi setup\n";
    Serial.println(usage_str);
}

static uint8_t getCli(char *data, uint8_t len)
{
    while (true)
    {
        if (Serial.available() > 0)
        {
            // Read characters into the buffer until a newline character is received or the buffer is full
            return Serial.readBytesUntil('\n', data, len - 1);
        }
    }
}

static wifi_state_t parseInput()
{
    uint8_t len = 2;
    char data[2] = {0};

    (void)getCli(data, len);

    if (data[0] !='-')
    {
        Serial.println("Inalid option");
        return WIFI_WAIT_COMMAND;
    }

    switch (data[1])
    {
    case 'h':
        return WIFI_WAIT_COMMAND;
    case 's':
        return WIFI_SCAN;
    case 'c':
        return WIFI_CONNECT;
    case 'd':
        return WIFI_CONNECT_DEFAULT_SSID;
    case 'e':
        return WIFI_EXIT;

    default:
        Serial.println("Inalid option");
        return WIFI_WAIT_COMMAND;
    }
}

void wifiStart()
{
    static bool is_wifi_connect = wifiConnect(WIFI_DEFAULT_SSID, WIFI_DEFAULT_PASS);

    if(!is_wifi_connect)
    {
        usage();
    }

    while (!is_wifi_connect)
    {
        switch (parseInput())
        {
        case WIFI_WAIT_COMMAND:
            usage();
            break;

        case WIFI_SCAN:
            wifiScan();
            break;

        case WIFI_CONNECT:
            wifiConnectCLI();
            break;

        case WIFI_CONNECT_DEFAULT_SSID:
            wifiConnect(WIFI_DEFAULT_SSID, WIFI_DEFAULT_PASS);
            break;

        case WIFI_EXIT:
            wifiIsConnected();
            is_wifi_connect = true;
            break;

        default:
            break;
        }
    }

    Serial.println("Wifi setup done!");
}

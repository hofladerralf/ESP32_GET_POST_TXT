# ESP32_GET_POST_TXT
reading a .txt-file from website and sending .txt-file to website <br>
### Change to your needs
//Network
const char* ssid = "xxx"; <br>
const char* password = "xxx"; <br>
String filedata = get_txtFile_fromHTTP("http://www.xxx.de/filedown.txt"); <br>
sendFile("xxx.de", "/upload.php", 80, "RPFile.txt", "/filedown.txt"); <br>

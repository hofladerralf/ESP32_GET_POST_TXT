# ESP32_GET_POST_TXT
reading a .txt-file from website and sending .txt-file to website <br>
### Change to your needs
:point_right: Network <br>
const char* ssid = "xxx"; <br>
const char* password = "xxx"; <br>
:point_right: location of file on server for downloading <br>
String filedata = get_txtFile_fromHTTP("http://www.xxx.de/filedown.txt"); <br>
:point_right: location of the .php-file on server for receiving file <br>
sendFile("xxx.de", "/upload.php", 80, "RPFile.txt", "/filedown.txt"); <br>

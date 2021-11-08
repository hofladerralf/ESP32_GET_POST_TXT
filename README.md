# ESP32_GET_POST_TXT
reading a .txt-file from website and sending .txt-file to website <br>
### Change to your needs
:point_right: Network <br>
const char* ssid = "xxx"; <br>
const char* password = "xxx"; <br>
:point_right: location of file on server for downloading <br>
String filedata = get_txtFile_fromHTTP("http://www.xxx.de/filedown.txt"); <br>
String filedata = get_txtFile_fromHTTP("https://www.xxx.de/filedown.txt"); is also possible, if SSL on server is enabled <br>
:point_right: location of the .php-file on server for receiving file <br>
sendFile("xxx.de", "/upload.php", 80, "RPFile.txt", "/filedown.txt"); <br>
### server:
:point_right: take care about the user of the .php-file on server for receiving file!! <br>
:point_right: disable automatic forwarding "http -> https", download can use https, upload on port 80 can use http <br>

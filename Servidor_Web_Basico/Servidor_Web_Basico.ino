/*
  Nombre del Proyecto: Servidor Web basico utilizando un ESP8266
  Descripción: Este codigo genera un servidor para el control del LED integrado a la placa NodeMcu v3
  Autor: Alejandro Galvis
  Fecha: 28/07/2024
  Versión: 1.1

  Hardware Utilizado:
    - NodeMcu v3
    - LED integrado al NodeMCU v3

  Descripción General:
  El codigo genera un mini Servidor Web para el control del LED integrado a la placa NodeMcu v3

  - Se incluye un archivo cabecera data.h donde se guardan varias redes WiFi con SSID y CONTRASEÑAS

  Funciones:
  - void setup(): Configura los pines, la comunicación serial y la conexión WiFi,IP donde se va alojar el servidor.
  - void loop(): Conexión de un cliente HTTP y generación del servidor para el control del LED integrado.

  
  Variables Globales:
  - const int ledPin: El pin al que está conectado el LED.
  - WiFiServer server(80): Objeto que apunta al puerto 80 TCP este puesto se utiliza para la navegación web http


  Ejemplo de Funcionamiento:
  - Programa que genera un servidor para controlar el encendido y el apagado del LED integrado en la placa Node MCU v3.

  Notas Adicionales:
  - En proxima revisión mejorar el diseño de la pagina web
*/

#if defined(ESP8266)
//Librerias para ESP8266
#include <ESP8266WiFi.h> //Libreria para utilizar protocolo TCP/IP
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;

#endif

#include "data.h" //archivo cabecera que contiene los SSID y CONTRASEÑAS

int ledPin = LED_BUILTIN; // Pin del LED
boolean state = false; //Estado del LED

const uint32_t WiFistanbyTime = 5000; //tiempo de espera de la conexión de la red WiFi
WiFiServer server(80); // Crear una instancia del servidor web en el puerto 80

void setup() {
  Serial.begin(115200);  // Inicializar la comunicación serie para depuración
  delay(10); //Pequeño retardo

  Serial.println("\nIniciando Multi Wifi");

  pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
  digitalWrite(ledPin, HIGH); // Apaga el LED

  // Conectar a la red WiFi
  wifiMulti.addAP(WIFI1_SSID, WIFI1_PASSWORD);
  wifiMulti.addAP(WIFI2_SSID, WIFI1_PASSWORD);

  WiFi.mode(WIFI_STA); //Configuración de la red WiFi como Modo Estación


  /**
    Conecta el módulo ESP8266 a la red WiFi especificada.
    Imprime el estado de la conexión y la dirección IP asignada.
  */

  Serial.print("Conectando a WiFi .."); // Esperar hasta que la conexión WiFi se establezca
  while (wifiMulti.run(WiFistanbyTime) != WL_CONNECTED) {
    Serial.print("*");
  }
  
   //Cuando se conecte lo imprimimos
  Serial.println("");
  Serial.println("WiFi Conectado");
   
  // Iniciamos el esp como servidor web
  server.begin();
  Serial.println("Servidor iniciado");
 
  // Imprimimos la dirección IP
  Serial.print("Direccion IP: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

}

void loop() {

  // El servidor siempre estará esperando a que se conecte un cliente
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   
  
  Serial.println("Nuevo cliente"); //Cuando un cliente se conecte vamos a imprimir que se conectó
  while(!client.available()){  //Esperamos a que el ciente mande una solicitud
    delay(1);
  }
   
  // Leemos la primer línea de la solicitud y la guardamos en la variable string request
  String request = client.readStringUntil('\r');
  Serial.println(request); //Imprimimos la solicitud
  client.flush(); //Descartamos los datos que se han escrito en el cliente y no se han leído
   
  // Relacionamos la solicitud
  if (request.indexOf("/LED=OFF") != -1){
    digitalWrite(ledPin, HIGH);
  }
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledPin, LOW);
  } 
  
  if (request.indexOf("/LED=BLINK") != -1){
    for(int i=0;i<10;i++)
    {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
    }
  }

  // Respuesta del servidor web
  
  client.println("HTTP/1.1 200 OK"); // La respuesta empieza con una linea de estado  
  client.println("Content-Type: text/html"); //Empieza el cuerpo de la respuesta indicando que el contenido será un documento html
  client.println(""); // Ponemos un espacio
  
  client.println("<!DOCTYPE HTML>"); //Indicamos el inicio del Documento HTML
  client.println("<html lang=\"en\">");
  client.println("<head>");
  client.println("<meta charset=\"UTF-8\">");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"); //Para que se adapte en móviles
  client.println("<title>Servidor Basico</title>");
  client.println("</head>");
  client.println("<body>");
  client.println("<br><br>");
  
  client.println("<h1 style=\"text-align: center;\">Servidor Web Basico</h1>");
  
  client.println("<p style=\"text-align: center;\">");
  client.println("Click <a href=\"/LED=ON\">Aqui</a> para encender LED integrado<br>"); //Oración que contiene un hipervínculo
  client.println("Click <a href=\"/LED=OFF\">Aqui</a> para apagar LED integrado<br>");
  client.println("Click <a href=\"/LED=BLINK\">Aqui</a> para parpadar el LED integrado<br> <br>");
  

  client.println("<button onclick=location.href=\"/LED=ON\"> Encender LED</button> <br> <br>"); // Botón sencillo que contiene hipervínculo
  client.println("<button onclick=location.href=\"/LED=OFF\" >Apagar LED </button> <br> <br>");
  client.println("<button onclick=location.href=\"/LED=BLINK\">Parpadear LED </button> <br> <br>");

  client.println("</p>");
  client.println("</body>");
  
  client.println("</html>"); //Terminamos el HTML
 
  delay(1);
  Serial.println("Cliente desconectado"); //Imprimimos que terminó el proceso con el cliente desconectado
  Serial.println("");
 
}

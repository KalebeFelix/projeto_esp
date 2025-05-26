#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>

// ------------------- CONFIGURE AQUI -------------------
const char* ssid       = "SUA_REDE";
const char* password   = "SUA_SENHA";

const char* backendIP  = "IP_DA_SUA_MAQUINA";   // <-- SUBSTITUA
const int   backendPort = 3000;             // porta do backend Node
// -----------------------------------------------------

ESP8266WebServer server(80);

// ---------- HTML pronto ----------
const char loginPage[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head><meta charset="utf-8"><title>Login</title>
<style>
 body{font-family:Arial;background:#f2f2f2;display:flex;justify-content:center;align-items:center;height:100vh;margin:0}
 .box{background:#fff;padding:30px;border-radius:8px;box-shadow:0 0 10px rgba(0,0,0,.1);width:300px;text-align:center}
 input{width:100%;padding:10px;margin:8px 0;border:1px solid #ccc;border-radius:4px}
 input[type=submit]{background:#4caf50;color:#fff;border:none;cursor:pointer}
 input[type=submit]:hover{background:#45a049}
 a{display:block;margin-top:10px;color:#555}
</style></head><body>
<div class="box">
 <h2>Factory Flow</h2>
 <form method="POST" action="/login">
  <input type="text" name="email"    placeholder="E-mail" required>
  <input type="password" name="senha" placeholder="Senha" required>
  <input type="submit" value="Entrar">
 </form>
 <a href="/registro">Novo? Registrar-se</a>
</div></body></html>
)HTML";

const char registroPage[] PROGMEM = R"HTML(
<!DOCTYPE html><html><head><meta charset="utf-8"><title>Registro</title>
<style>
 body{font-family:Arial;background:#f2f2f2;display:flex;justify-content:center;align-items:center;height:100vh;margin:0}
 .box{background:#fff;padding:30px;border-radius:8px;box-shadow:0 0 10px rgba(0,0,0,.1);width:300px;text-align:center}
 input{width:100%;padding:10px;margin:8px 0;border:1px solid #ccc;border-radius:4px}
 input[type=submit]{background:#2196f3;color:#fff;border:none;cursor:pointer}
 input[type=submit]:hover{background:#1976d2}
 a{display:block;margin-top:10px;color:#555}
</style></head><body>
<div class="box">
 <h2>Registrar</h2>
 <form method="POST" action="/registrar">
  <input type="text" name="email"    placeholder="E-mail" required>
  <input type="password" name="senha" placeholder="Senha" required>
  <input type="submit" value="Registrar">
 </form>
 <a href="/">Voltar ao Login</a>
</div></body></html>
)HTML";


// ---- funções auxiliares ----------
bool postJSON(const String& route, const String& json, String& resp, int& code) {
  WiFiClient client;  
  HTTPClient http;

  String url = "http://" + String(backendIP) + ":" + String(backendPort) + route;
  http.begin(client, url);  

  http.addHeader("Content-Type", "application/json");

  code = http.POST(json);
  resp = http.getString();

  http.end();
  return (code == 200);
}

// ---------- handlers --------------
void handleRoot() { server.send(200, "text/html", loginPage); }

void handleRegistroForm() { server.send(200, "text/html", registroPage); }

void handleRegistrar() {
  String email = server.arg("email");
  String senha = server.arg("senha");

  String payload, resp;
  int code;
  payload = "{\"email\":\"" + email + "\",\"password\":\"" + senha + "\"}";

  bool ok = postJSON("/registrar", payload, resp, code);

  if (ok) {
    server.send(200, "text/html",
      "<h1>Registrado com sucesso!</h1><a href='/'>Fazer login</a>");
  } else {
    server.send(400, "text/html",
      "<h1>Erro ao registrar (" + String(code) + ")</h1><pre>" + resp + "</pre><a href='/registro'>Voltar</a>");
  }
}

void handleLogin() {
  String email = server.arg("email");
  String senha = server.arg("senha");

  String payload = "{\"email\":\"" + email + "\",\"password\":\"" + senha + "\"}";
  String resp;
  int code;
  bool ok = postJSON("/login", payload, resp, code);

  if (ok) {
    String page = "<!DOCTYPE html><html><head><meta charset='utf-8'><title>Bem-vindo</title>"
                  "<style>body{font-family:Arial;text-align:center;padding:40px;background:#f9f9f9}"
                  "h1{color:#4caf50}</style></head><body>"
                  "<h1>Bem-vindo, " + email + "!</h1>"
                  "<p>Você está logado sistema de PCM <strong>Factory Flow</strong>.</p>"
                  "</body></html>";
    server.send(200, "text/html", page);
  } else {
    server.send(401, "text/html",
      "<h1>Credenciais inválidas (" + String(code) + ")</h1><pre>" + resp + "</pre><a href='/'>Tentar novamente</a>");
  }
}
// ----------------------------------

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print('.'); }
  Serial.println("\nIP: " + WiFi.localIP().toString());

  if (MDNS.begin("factoryflow")) {
    Serial.println("mDNS iniciado: acesse http://esp8266.local");
  }else {
    Serial.println("Erro ao iniciar mDNS");
  }

  server.on("/",          HTTP_GET,  handleRoot);
  server.on("/login",     HTTP_POST, handleLogin);
  server.on("/registro",  HTTP_GET,  handleRegistroForm);
  server.on("/registrar", HTTP_POST, handleRegistrar);

  server.begin();
  Serial.println("Servidor ESP pronto em http://" + WiFi.localIP().toString());
}

void loop() {
  server.handleClient();
  MDNS.update();
}


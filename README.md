<h1 align="center">🔐 ESP8266 + Supabase Auth + Node.js Backend</h1>

<p align="center">
  Projeto de autenticação completo com <strong>ESP8266</strong> servindo o front-end, 
  <strong>Node.js</strong> como backend local e <strong>Supabase</strong> como sistema de autenticação.
</p>

---

## 📸 Visão Geral

> 📡 O ESP8266 hospeda uma página de login/cadastro acessível pela rede local (via mDNS).
>
> 🧠 O Backend (Node.js) roda no seu PC e faz a ponte entre o ESP e o Supabase.
>
> 🔐 O Supabase realiza o cadastro, login e validação de usuários.

---

## 🧰 Tecnologias Utilizadas

- ⚙️ **ESP8266** (ESP-01, NodeMCU, Wemos, etc.)
- 🌐 **Node.js + Express**
- 🧾 **Supabase** (Auth)
- 📡 **mDNS** (acesso via `esp.local`)
- 🔄 **HTTP REST**
- 🖥️ **HTML/CSS** (frontend simples embutido no ESP)

---

## 🚀 Como Funciona

| Camada       | Componente                     | Papel                                                                 |
|--------------|--------------------------------|-----------------------------------------------------------------------|
| **Frontend** | Servido diretamente pelo ESP8266 via mDNS (ex: `http://esp.local`) | Página HTML de login e cadastro de usuários                           |
| **Backend**  | Node.js rodando no seu PC (ex: `http://192.168.1.100:3000`)        | Recebe requisições do ESP, acessa a API do Supabase e retorna resposta |
| **Auth**     | Supabase                                                            | Realiza o cadastro, login e verificação do usuário                     |
s
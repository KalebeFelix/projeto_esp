const express = require('express');
const cors = require('cors');
const bodyParser = require('body-parser');
const { createClient } = require('@supabase/supabase-js');
const os = require('os');

const app = express();
const PORT = 3000;

// Substitua com suas credenciais reais do Supabase
const SUPABASE_URL = 'SUA_URL';
const SUPABASE_KEY = 'SUA_KEY'
const supabase = createClient(SUPABASE_URL, SUPABASE_KEY);

// Middlewares
app.use(cors());
app.use(bodyParser.json());

// Middleware para log de requisições
app.use((req, res, next) => {
  console.log(`[${new Date().toISOString()}] ${req.method} ${req.originalUrl}`);
  next();
});

// Rota de teste
app.get('/', (req, res) => {
  res.send('Servidor rodando!');
});

// Cadastro
app.post('/registrar', async (req, res) => {
  const { email, password } = req.body;
  console.log("Recebido (registrar):", req.body);

  const { data, error } = await supabase.auth.signUp({ email, password });

  if (error) return res.status(400).json({ success: false, error: error.message });
  res.json({ success: true, user: data.user });
});

// Login
app.post('/login', async (req, res) => {
  const { email, password } = req.body;
  console.log("Recebido (login):", req.body);

  const { data, error } = await supabase.auth.signInWithPassword({ email, password });

  if (error) return res.status(401).json({ success: false, error: error.message });
  res.json({ success: true, session: data.session, user: data.user });
});

// Validação de token
app.get('/usuario', async (req, res) => {
  const token = req.headers.authorization?.split(' ')[1];
  if (!token) return res.status(401).json({ error: 'Token não fornecido' });

  const supabaseWithToken = createClient(SUPABASE_URL, SUPABASE_KEY, {
    global: { headers: { Authorization: `Bearer ${token}` } },
  });

  const { data: { user }, error } = await supabaseWithToken.auth.getUser();
  if (error) return res.status(400).json({ error: error.message });

  res.json({ user });
});

// Obter IP local para exibir no terminal
const interfaces = os.networkInterfaces();
let localIP = 'localhost';
for (let iface of Object.values(interfaces)) {
  for (let config of iface) {
    if (config.family === 'IPv4' && !config.internal) {
      localIP = config.address;
    }
  }
}

// Inicia o servidor
app.listen(PORT, '0.0.0.0', () => {
  console.log(`✅ Servidor rodando em http://${localIP}:${PORT}`);
});


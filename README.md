# Sistema de Monitoramento IoT da Qualidade do Ar (ODS 3)

Este projeto implementa uma arquitetura IoT de ponta a ponta para monitoramento contínuo da qualidade do ar em ambientes acadêmicos (Sala 101 e Sala 202). O sistema visa prevenir a degradação microclimática e está alinhado ao **Objetivo de Desenvolvimento Sustentável 3 (Meta 3.9) da ONU**, promovendo saúde e bem-estar através da mitigação da poluição interna.

## 📺 Apresentação do Projeto
Assista ao vídeo explicativo detalhando o funcionamento da arquitetura e a integração entre os componentes:
▶️ **[Assistir no YouTube](https://youtu.be/jRlgJJYUdMA)**

## 📊 Dashboards ao Vivo
Você pode visualizar os painéis de monitoramento analítico operando em tempo real através dos links abaixo:
- **[Dashboard Sala 101 - Grafana](https://generousdeer2252.grafana.net/public-dashboards/6ec06cc00d424e88a69711a7f98c2fed)**
- **[Dashboard Sala 202 - Grafana](https://generousdeer2252.grafana.net/public-dashboards/16b65d20c74a4e7bb35838cb4f7d96c7)**

## 🏗️ Arquitetura e Tecnologias Utilizadas
A solução foi construída utilizando microsserviços desacoplados e mensageria leve:
* **Edge/Hardware (Simulação Wokwi):** Microcontroladores ESP32 programados em C++ integrados a sensores DHT22 (Temperatura/Umidade) e MQ-135 (CO2).
* **Transporte de Dados:** Protocolo MQTT via broker público (HiveMQ).
* **Orquestração e Integração:** Node-RED para processamento de payloads, roteamento condicional e acionamento de APIs.
* **Banco de Dados (Time-Series):** InfluxDB Cloud para armazenamento de histórico via Line Protocol.
* **Visualização Analítica:** Dashboards dinâmicos no Grafana Cloud utilizando linguagem Flux.
* **Notificações de Emergência:** Integração com a API de Bots do Telegram para emissão de alertas críticos em tempo real.

## 🚀 Funcionalidades
- **Telemetria em Tempo Real:** Coleta contínua de CO2, Temperatura e Umidade com envio frequente.
- **Acionamento Automático:** LEDs RGB e Buzzers (simulados) respondem localmente ao status do ambiente via subscrição MQTT.
- **Alertas Inteligentes:** Notificações instantâneas enviadas via Telegram apenas quando parâmetros críticos (CO2 > 2000 PPM ou Temp >= 50°C) são atingidos, evitando spam.
- **Tabelas Comparativas:** Monitoramento simultâneo das duas salas em um único painel cruzado no Grafana.

## 📂 Como importar este projeto

**1. Hardware (Wokwi):**
Abra o site [Wokwi](https://wokwi.com/) e crie um novo projeto ESP32. Substitua os arquivos padrão pelos arquivos encontrados nas pastas do repositório (`diagram.json`, `libraries.txt` e o código `.ino` ou `.cpp`). Isso carregará automaticamente o circuito elétrico e as bibliotecas corretas.

**2. Fluxo Node-RED:**
No Node-RED, vá no menu principal > **Importar**, e selecione o arquivo `node-red-flow.json` disponibilizado neste repositório. 
*Atenção:* Você precisará atualizar o nó de função com o seu próprio **Token do InfluxDB** e seu **Bot Token do Telegram**.

---
*Projeto desenvolvido por Pedro Henrique Mendes Vieira Gonçalves - Universidade Presbiteriana Mackenzie.*

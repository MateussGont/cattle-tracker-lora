param()

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot

function Write-Step($msg) {
    Write-Host ""
    Write-Host "== $msg ==" -ForegroundColor Cyan
}

function Test-DockerReady {
    $prevPref = $ErrorActionPreference
    $ErrorActionPreference = "SilentlyContinue"
    docker info *> $null
    $ErrorActionPreference = $prevPref
    return ($LASTEXITCODE -eq 0)
}

Write-Host "Cattle Tracker - inicializando o sistema" -ForegroundColor Yellow

# 1. Docker Desktop --------------------------------------------------------
Write-Step "Docker"
if (Test-DockerReady) {
    Write-Host "Docker ja esta rodando." -ForegroundColor Green
} else {
    $dockerExe = "C:\Program Files\Docker\Docker\Docker Desktop.exe"
    if (Test-Path $dockerExe) {
        Write-Host "Abrindo Docker Desktop..."
        Start-Process $dockerExe | Out-Null
    } else {
        Write-Host "Nao encontrei o Docker Desktop instalado em '$dockerExe'." -ForegroundColor Red
        Write-Host "Abra o Docker manualmente e rode este script de novo."
        Read-Host "Pressione Enter para fechar"
        exit 1
    }

    Write-Host "Aguardando o Docker inicializar (ate 2 minutos)..."
    $elapsed = 0
    $ready = $false
    while ($elapsed -lt 120) {
        if (Test-DockerReady) { $ready = $true; break }
        Start-Sleep -Seconds 3
        $elapsed += 3
    }

    if (-not $ready) {
        Write-Host "O Docker nao respondeu a tempo. Tente rodar este script de novo em alguns instantes." -ForegroundColor Red
        Read-Host "Pressione Enter para fechar"
        exit 1
    }
    Write-Host "Docker pronto." -ForegroundColor Green
}

# 2. Infra: Postgres + Mosquitto -------------------------------------------
Write-Step "Infraestrutura (PostgreSQL + Mosquitto)"
Push-Location (Join-Path $root "infra")
$passwdFile = Join-Path $root "infra\mosquitto\passwd"
if (-not (Test-Path $passwdFile)) {
    Write-Host "Criando credencial MQTT local (gateway/change-me-local-mqtt)..."
    docker run --rm -v "${PWD}/mosquitto:/mosquitto/config" eclipse-mosquitto:2 `
        mosquitto_passwd -b -c /mosquitto/config/passwd gateway "change-me-local-mqtt"
}
docker compose up -d --wait --wait-timeout 60
Pop-Location

# 3. Dependencias e ambiente -----------------------------------------------
Write-Step "Dependencias e configuracao local"
if (-not (Test-Path (Join-Path $root "backend\.env"))) {
    Copy-Item (Join-Path $root "backend\.env.example") (Join-Path $root "backend\.env")
    Write-Host "backend/.env criado a partir do exemplo." -ForegroundColor Yellow
}
if (-not (Test-Path (Join-Path $root "node_modules"))) {
    Push-Location $root
    npm install
    Pop-Location
}

Write-Host "Aplicando migrations e seed idempotente..."
Push-Location $root
npm run db:migrate
npm run db:seed
Pop-Location

# 4. Backend -----------------------------------------------------------------
Write-Step "Backend (http://localhost:3000)"
Start-Process powershell -ArgumentList @(
    "-NoExit", "-Command",
    "Set-Location '$root\backend'; Write-Host 'Backend - Cattle Tracker' -ForegroundColor Cyan; npm run dev"
) | Out-Null

# 5. Frontend ------------------------------------------------------------
Write-Step "Frontend (http://localhost:5173)"
Start-Process powershell -ArgumentList @(
    "-NoExit", "-Command",
    "Set-Location '$root\frontend'; Write-Host 'Frontend - Cattle Tracker' -ForegroundColor Cyan; npm run dev"
) | Out-Null

# 6. Abrir o navegador --------------------------------------------------
Write-Step "Abrindo o navegador"
Start-Sleep -Seconds 6
Start-Process "http://localhost:5173"

Write-Host ""
Write-Host "Tudo iniciado." -ForegroundColor Green
Write-Host "Backend:  http://localhost:3000"
Write-Host "Frontend: http://localhost:5173"
Write-Host ""
Write-Host "Os servidores continuam rodando nas outras janelas do PowerShell que abriram."
Write-Host "Para parar: feche aquelas janelas e rode 'docker compose down' dentro de infra/ (ou use scripts\stop-app.ps1)."
Read-Host "Pressione Enter para fechar esta janela"

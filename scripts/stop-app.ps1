param()

$ErrorActionPreference = "Continue"
$root = Split-Path -Parent $PSScriptRoot

Write-Host "Cattle Tracker - parando o sistema" -ForegroundColor Yellow

Write-Host ""
Write-Host "Encerrando backend (npm run dev / tsx watch)..."
Get-CimInstance Win32_Process -Filter "Name = 'node.exe'" |
    Where-Object { $_.CommandLine -match "tsx" -and $_.CommandLine -match "server\.ts" } |
    ForEach-Object {
        Write-Host "  matando PID $($_.ProcessId)"
        Stop-Process -Id $_.ProcessId -Force -ErrorAction SilentlyContinue
    }

Write-Host "Encerrando frontend (vite)..."
Get-CimInstance Win32_Process -Filter "Name = 'node.exe'" |
    Where-Object { $_.CommandLine -match "vite" } |
    ForEach-Object {
        Write-Host "  matando PID $($_.ProcessId)"
        Stop-Process -Id $_.ProcessId -Force -ErrorAction SilentlyContinue
    }

Write-Host ""
Write-Host "Derrubando containers (Postgres + Mosquitto)..."
Push-Location (Join-Path $root "infra")
docker compose down
Pop-Location

Write-Host ""
Write-Host "Sistema parado." -ForegroundColor Green
Read-Host "Pressione Enter para fechar"

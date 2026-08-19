@echo off
title Cattle Tracker - parar
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0scripts\stop-app.ps1"

clear;
global app

tamPacote = 1000;
energiaSec = 1;
app = appMedicao;
pause(1)
app.tamTensao = tamPacote;
app.tamCorrente = tamPacote;
app.tamTempTP = tamPacote;
app.tamIlumin = tamPacote;
app.tamPotencia = tamPacote;
app.tamEnergia = 0;

i = 0;
while(1)
    i = i+1;
    strAtual = sin(i/100) + ";" + cos(i/100) + ";" + 30 + ";" + 700;
    teste(tamPacote, energiaSec, i, strAtual);
    
    if mod(i, tamPacote+2)==0
        pause(0.5)
    end
end
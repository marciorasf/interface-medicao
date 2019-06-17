function leitura(obj,event)

global PortaCOM;
global i tensao corrente ilumin potencia tempTP tamPacote tamFatia energiaSec app;
global coeffConv ganhoTensao offsetTensao ganhoCorrente offsetCorrente ganhoIlumin ganhoTempTP;

i = i+1;
strAtual = fscanf(PortaCOM);
tempLM = app.tempLM;

%disp(strAtual);

if i == tamPacote+1
    potencia = (tensao.*corrente)*10e-3;
    app.tensao = tensao;
    app.corrente = corrente;
    app.tempTP = tempLM + tempTP;
    app.ilumin = ilumin;
    app.potencia = potencia;
    app.tamEnergia = app.tamEnergia+1;
    
    if app.tamEnergia == 1
        app.energia(app.tamEnergia) = 2*energiaSec;
    else
        app.energia(app.tamEnergia) = app.energia(app.tamEnergia - 1) + 2*energiaSec;
    end
    
    switch app.flag
        case 1
            app.TensaoRefresh();
        case 2
            app.CorrenteRefresh();
        case 3
            app.TempTPRefresh();
        case 4
            app.IluminRefresh();
        case 5
            app.PotenciaRefresh();
        case 6
            app.EnergiaRefresh();
    end
    
    i = mod(i,tamPacote);
end

if (i > 0)
    strSeparado = strsplit(strAtual,';');
    strSepConv = str2double(strSeparado);
    tensao(i) = (strSepConv(1,1)*coeffConv - offsetTensao)*ganhoTensao;
    corrente(i)= (strSepConv(1,2)*coeffConv - offsetCorrente)*ganhoCorrente;
    tempTP(i)= strSepConv(1,3)*coeffConv*ganhoTempTP;
    ilumin(i)= strSepConv(1,4)*coeffConv*ganhoIlumin;
end

end
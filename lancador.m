clear;

global PortaCOM;
global i tamPacote energiaSec app;
global coeffConv ganhoTensao offsetTensao ganhoCorrente offsetCorrente ganhoIlumin ganhoTempTP;

coeffConv = 5/1023;

offsetTensao = 2.53;
ganhoTensao = 109.5;

offsetCorrente = 2.60;
ganhoCorrente = 60.1;

ganhoTempTP = 1055/92;

ganhoIlumin = 1055/3;

energiaSec = (117.3984)*(2.778e-7);

tamPacote = 200;
i = -tamPacote;

app = appMedicao;
app.tamTensao = tamPacote;
app.tamCorrente = tamPacote;
app.tamTempTP = tamPacote;
app.tamIlumin = tamPacote;
app.tamPotencia = tamPacote;
app.tamEnergia = 0;

delete(instrfindall);
PortaCOM = serial('COM3','BaudRate',115200,'DataBits',8);
PortaCOM.BytesAvailableFcnCount = 500;
PortaCOM.BytesAvailableFcnMode = 'byte';
PortaCOM.Terminator = 'CR';
PortaCOM.BytesAvailableFcn = @leitura;
fopen(PortaCOM);
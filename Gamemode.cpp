#include <Biblioteca>

main() {}

enum Gamer{
	Conta,
	Senha,
	Grana,
	Level,
	Exp,
	LvlWanted,
	Cadastrado,
	CidadeNatal,
	Administrador,
	Float:UltimaPosX,
	Float:UltimaPosY,
	Float:UltimaPosZ,
	Float:UltimoAngulo
}; new EPlayer[MAX_PLAYERS][Gamer];

//Ammu-Nation LS;
new ComproArma[MAX_PLAYERS];

new AmmuLS;

new QualArmaCompro[MAX_PLAYERS][4];

// Correspondem ao DIALOG de OPTIONS DAS ARMAS ( Na hora de escolher );

#define DIALOG_PISTOL_SILENCE 20
#define DIALOG_PISTOL_EAGLE 21
#define DIALOG_PISTOL_COLT45 22
#define DIALOG_WEAPON_MP5 23

// Correspondem ao DIALOG de RETIRADA DAS ARMAS ( CMD:retirar );

#define DIALOG_MP5_SILENCE 24
#define DIALOG_MP5_DESERT 25
#define DIALOG_MP5_MM4 26
#define DIALOG_PISTOL_DESERT 27

//

#define PRESSED(%0) \
	(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
#pragma tabsize 0

	new Pickup_AmmuLS[2]
	;

// Var - Veículos Publicos(ROUBADOS)
	new CarsPublic[11];


/* Variaveis Sob
	- Cadastro(Login/Registro);
*/
new StrAutentic[500],
	ErrouSenha[MAX_PLAYERS],
	ReasonMaxFalhas[MAX_PLAYERS]
	;

/* Variaveis
	- Veículos(CMD:CriarVeiculo);
*/
new criarc[MAX_PLAYERS];
new Text3D:criarcText[MAX_PLAYERS];
//
new id,
	Float:_x,
	Float:_y,
	Float:_z
;

new String[500];

//	- Sistema de Spawn Randomico

new LocalSpawn[][] = {
	"Dillimore",
	"PalominoCreek",
	"Blueberry",
	"AngelPine",
	"FortCarson",
	"Montgomery"
};

#define DIALOG_AUTENTIC 1
#define DIALOG_SEMCONTA 2
#define DIALOG_KICKMAXERRORS 3
#define DIALOG_SPAWN-DILLI 4

public OnGameModeInit()
{
	SetGameModeText("Indefinido");
	AddPlayerClass(0, 1958.3783, 1343.1572, 15.3746, 269.1425, 0, 0, 0, 0, 0, 0);

	DisableInteriorEnterExits();
	EnableStuntBonusForAll(0);
	UsePlayerPedAnims();
	ShowPlayerMarkers(0);

	ConnectNPC("Dave","AmmuLS");

	r@CarregarVehPublicos();
    CarregarAmmuLS();

    Pickup_AmmuLS [ 0 ] = CreatePickup (1242 , 0 , 1368.3445,-1279.6459,13.5469 , - 1 ) ;
        Create3DTextLabel("{FA8072}Ammu-Nation - LS\n{BEBEBE}Pressione '{FFFF00}F{BEBEBE}' para Entrar!", 0x008080FF, 1368.3445,-1279.6459,13.5469, 15.0, 0, 0);
	Pickup_AmmuLS [ 1 ] = CreatePickup (1242 , 0 , 316.3768,-169.8894,999.6010 , - 1 ) ;
        Create3DTextLabel("{FA8072}Ammu-Nation - LS\n{BEBEBE}Pressione '{FFFF00}F{BEBEBE}' para Sair!", 0x008080FF, 316.3768,-169.8894,999.6010, 15.0, 0, 0);

    Create3DTextLabel("{BEBEBE}Balcão{FA8072} Ammu-Nation\n{BEBEBE}Retire suas Armas", 0x008080FF, 312.4777,-165.7246,999.6010, 10.0, 0, 0);

	return 1;
}

public OnGameModeExit(){
	DOF2_Exit();
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    if(IsPlayerNPC(playerid)) return 1;
	SetPlayerPos(playerid, 1834.80017, -1311.05750, 114.80832);
	SetPlayerCameraPos(playerid, 1841.9995, -1312.6560, 114.8578);
	SetPlayerCameraLookAt(playerid, 1840.9919, -1312.6553, 114.8724);

	if(ErrouSenha[playerid] >= 1) return ErrouDados(playerid);

	if(DOF2_FileExists(ObterConta(playerid))){
	    format(StrAutentic, sizeof(StrAutentic), "{BEBEBE}Bem Vindo(a) ao {FFFF00}------BEBEBE}!\n\nA Conta {FA8072}%s {BEBEBE}foi encontrada no sistema, autentique sua senha abaixo para prosseguir.\n\n{FFD700}Jamais Passe sua senha para terceiros, nós não nos responsabilizamos por isso!", pName(playerid));
	    ShowPlayerDialog(playerid, DIALOG_AUTENTIC, DIALOG_STYLE_PASSWORD, "Autenticar-se no Sistema!", StrAutentic, "Autenticar", "Cancelar");
	}
	else{
	    format(StrAutentic, sizeof(StrAutentic), "{BEBEBE}Bem Vindo(a) ao {FFFF00}------{BEBEBE}!\n\nA Conta {FA8072}%s {BEBEBE}não foi encontrada no sistema, digite uma senha abaixo para prosseguir.\n\n{FFD700}Jamais Passe sua senha para terceiros, nós não nos responsabilizamos por isso!\n\n{FF0000}Preencha o campo abaixo!", pName(playerid));
	    ShowPlayerDialog(playerid, DIALOG_SEMCONTA, DIALOG_STYLE_PASSWORD, "Autenticar-se no Sistema!", StrAutentic, "Autenticar", "Cancelar");
	}
	return 0;
}

public OnPlayerConnect(playerid)
{

	// Remove Ammu-LS ~ Aonde fica as Armas nas mesas!
    RemoveBuildingForPlayer(playerid, 1885, 310.1250, -166.2109, 998.5547, 0.25);
	RemoveBuildingForPlayer(playerid, 1885, 315.2422, -169.8984, 998.5547, 0.25);
	RemoveBuildingForPlayer(playerid, 18105, 312.9844, -163.2500, 1000.5547, 0.25);
	RemoveBuildingForPlayer(playerid, 1886, 318.8047, -158.9297, 1003.2266, 0.25);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    r@SalvarDados(playerid);
    r@SalvarPos(playerid);
    //SalvarConquistas(playerid);
	return 1;
}

public OnPlayerSpawn(playerid)
{
	if(IsPlayerNPC(playerid)){
	new npcname[MAX_PLAYER_NAME];
	GetPlayerName(playerid, npcname, sizeof(npcname));
	if(!strcmp(npcname, "Dave", true)){
		SetPlayerInterior(playerid, 6);
		SetPlayerPos(playerid, 312.5624,-168.3129,999.5938);
		SetPlayerFacingAngle(playerid, 5.8915);

	    new Text3D:label = Create3DTextLabel("{BEBEBE}Vendedor {FA8072}Ammu-Nation\n{BEBEBE}Dave!", 0x008080FF, 312.5624,-168.3129,999.5938, 10.0, 0);
	    Attach3DTextLabelToPlayer(label, playerid, 0.0, 0.0, 0.2);

		}
	}
	return 1;
}

public OnPlayerDeath(playerid, killerid, reason)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}

public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(oldstate == PLAYER_STATE_ONFOOT && newstate == PLAYER_STATE_DRIVER){
	new CarModel = GetPlayerVehicleID(playerid);
	    if(CarModel >= CarsPublic[0] && CarModel <= CarsPublic[10]) {
	        //if(AdquiriuRoubarCars[playerid] == 1) return 1;
            if(strcmp("Roubando Carros", DOF2_GetString(ObterConquistas(playerid), "Conquista01"))) {
			SetPlayerWantedLevel(playerid, Estrelas(playerid)+2);
			SendClientMessage(playerid, -1, "");

			SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {BEBEBE}Você acaba de roubar um carro e você será procurado nacional!");
			SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {BEBEBE}Você poderá roubar carros que não estiverem trancados.");
			SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {FA8072}Conquista Desbloqueada! {1E90FF}- {FFD700}Roubando Carros");

    	//	AdquiriuRoubarCars[playerid] = 1;

			DOF2_SetString(ObterConquistas(playerid), "Conquista01", "Roubando Carros");
			DOF2_SaveFile();
			}
		}
	}
	return 1;
}


public OnRconCommand(cmd[])
{
	return 1;
}

public OnPlayerRequestSpawn(playerid)
{
    SpawnPlayer(playerid);
	return 1;
}


public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

CMD:goto(playerid){
		SetPlayerPos(playerid, 317.1362,-167.2568,999.5938);
		SetPlayerFacingAngle(playerid, 16.1934);
		SetPlayerInterior(playerid, 6);
		return 1;
}

CMD:retirar(playerid){
    if (!IsPlayerInRangeOfPoint(playerid, 1.0, 312.6482,-165.8328,999.6010))  return SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| Você não está no balcão da Ammu-Nation! ");
	if(QualArmaCompro[playerid][0] == 1 && QualArmaCompro[playerid][3] == 1){ // Silence + MP5
	    ShowPlayerDialog(playerid, DIALOG_MP5_SILENCE, DIALOG_STYLE_MSGBOX, "{BEBEBE}Retirar Armas", "{BEBEBE}O Pedido abaixo foi encaminhado pelo atendente, confira e clica em {FFFF00}pagar {BEBEBE}para retirar suas armas!\n\n{BEBEBE}Armas: {FA8072}MP5 + 9MM\n{BEBEBE}Balas: {FA8072}MP5: 10 Balas & 9MM 10 Balas\n{BEBEBE}Valor á Pagar: {FA8072}R$1700,00\n\n{BEBEBE}Clique em {FFFF00}Comprar {BEBEBE}para finalizar a compra ou {FFFF00}Cancelar {BEBEBE}para desistir da compra!",
		"Comprar", "Cancelar");

	}
	else if(QualArmaCompro[playerid][3] == 1 && QualArmaCompro[playerid][1] == 1){ // MP5 + DESERT
         ShowPlayerDialog(playerid, DIALOG_MP5_DESERT, DIALOG_STYLE_MSGBOX, "{BEBEBE}Retirar Armas", "{BEBEBE}O Pedido abaixo foi encaminhado pelo atendente, confira e clica em {FFFF00}pagar {BEBEBE}para retirar suas armas!\n\n{BEBEBE}Armas: {FA8072}MP5 + Desert\n{BEBEBE}Balas: {FA8072}MP5: 10 Balas & Desert 10 Balas\n{BEBEBE}Valor á Pagar: {FA8072}R$1980,00\n\n{BEBEBE}Clique em {FFFF00}Comprar {BEBEBE}para finalizar a compra ou {FFFF00}Cancelar {BEBEBE}para desistir da compra!", "Comprar", "Cancelar");
	}
	else if(QualArmaCompro[playerid][2] == 1 && QualArmaCompro[playerid][3] == 1){ // MM4 SILENCE + MP5
	    ShowPlayerDialog(playerid, DIALOG_MP5_MM4, DIALOG_STYLE_MSGBOX, "{BEBEBE}Retirar Armas", "{BEBEBE}O Pedido abaixo foi encaminhado pelo atendente, confira e clica em {FFFF00}pagar {BEBEBE}para retirar suas armas!\n\n{BEBEBE}Armas: {FA8072}MP5 + 9MM\n{BEBEBE}Balas: {FA8072}MP5: 10 Balas & 9MM 10 Balas\n{BEBEBE}Valor á Pagar: {FA8072}R$1930,00\n\n{BEBEBE}Clique em {FFFF00}Comprar {BEBEBE}para finalizar a compra ou {FFFF00}Cancelar {BEBEBE}para desistir da compra!", "Comprar", "Cancelar");
	}
	else if(QualArmaCompro[playerid][1] == 1){
	    GivePlayerWeapon(playerid, 24, 10); // desert
	    QualArmaCompro[playerid][1] = 0;

	    ShowPlayerDialog(playerid, DIALOG_PISTOL_DESERT, DIALOG_STYLE_MSGBOX, "", "", "", "");
	}
	return 1;
}

CMD:trein(playerid) return SetPlayerInterior(playerid, 6), SetPlayerPos(playerid, 306.20869,-159.1122,999.10938), 0;
CMD:vaza(playerid) return SetPlayerInterior(playerid, 0), SetPlayerPos(playerid, 1368.3445,-1279.6459,13.5469);

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if (newkeys == 16 && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
        if (IsPlayerInRangeOfPoint(playerid, 1.0, 1368.3445,-1279.6459,13.5469)) {
			SetPlayerInterior(playerid, 6);
		    SetPlayerPos(playerid, 316.3768,-169.8894,999.6010);
		    AmmuLS = CheckPoint_Create(312.4777,-165.7246,999.6010, 1.0, 10);
		}
	}
 if (newkeys == 16 && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT) {
        if (IsPlayerInRangeOfPoint(playerid, 1.0, 316.3768,-169.8894,999.6010)) {
            SetPlayerInterior(playerid, 0);
            SetPlayerPos(playerid, 1368.3445,-1279.6459,13.5469);
            CheckPoint_Del(AmmuLS);
		}
	}
	//Ammu LS
	if (PRESSED(KEY_SPRINT)){
	    if (IsPlayerInRangeOfPoint(playerid, 0.5, 310.8896,-164.0021,999.5938)){
	        if(ComproArma[playerid] == 1) return SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {BEBEBE}Aguarde alguns instantes antes de comprar outra arma novamente!");
	        ShowPlayerDialog(playerid, DIALOG_PISTOL_SILENCE, DIALOG_STYLE_MSGBOX, "{BEBEBE}Ammu-Nation","{BEBEBE}Você solicitou um pedido para a arma {FFFF00}Pistola c/ Silenciador!\n{BEBEBE}Confira os dados abaixo e confirme o pedido!\n\n{FA8072}Arma: {BEBEBE}Pistola\n{FA8072}Option:{BEBEBE} Silenciador\n{FA8072}Munição: {BEBEBE}10 Balas\n{FA8072}Valor Total: {FFFF00}450,00","Comprar","Cancelar");
		}
	}
	else if(IsPlayerInRangeOfPoint(playerid, 0.5, 309.3380,-162.9703,999.5938)){
        	if(ComproArma[playerid] == 1) return SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {BEBEBE}Aguarde alguns instantes antes de comprar outra arma novamente!");
            ShowPlayerDialog(playerid, DIALOG_PISTOL_EAGLE, DIALOG_STYLE_MSGBOX, "{BEBEBE}Ammu-Nation","{BEBEBE}Você solicitou um pedido para a arma {FFFF00}Desert Eagle!\n{BEBEBE}Confira os dados abaixo e confirme o pedido!\n\n{FA8072}Arma: {BEBEBE}Pistola\n{FA8072}Option:{BEBEBE} n/a\n{FA8072}Munição: {BEBEBE}10 Balas\n{FA8072}Valor Total: {FFFF00}730,00","Comprar","Cancelar");

	}
	else if(IsPlayerInRangeOfPoint(playerid, 0.5, 309.0084,-161.7964,999.5938)){
        	if(ComproArma[playerid] == 1) return SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {BEBEBE}Aguarde alguns instantes antes de comprar outra arma novamente!");
            ShowPlayerDialog(playerid, DIALOG_PISTOL_COLT45, DIALOG_STYLE_MSGBOX, "{BEBEBE}Ammu-Nation","{BEBEBE}Você solicitou um pedido para a arma {FFFF00}Colt 45!\n{BEBEBE}Confira os dados abaixo e confirme o pedido!\n\n{FA8072}Arma: {BEBEBE}Pistola\n{FA8072}Option:{BEBEBE} Dual\n{FA8072}Munição: {BEBEBE}10 Balas\n{FA8072}Valor Total: {FFFF00}680,00","Comprar","Cancelar");

	}
	else if(IsPlayerInRangeOfPoint(playerid, 0.5, 310.9264,-160.6841,999.5938)){
        	if(ComproArma[playerid] == 1) return SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {BEBEBE}Aguarde alguns instantes antes de comprar outra arma novamente!");
            ShowPlayerDialog(playerid, DIALOG_WEAPON_MP5, DIALOG_STYLE_MSGBOX, "{BEBEBE}Ammu-Nation","{BEBEBE}Você solicitou um pedido para a arma {FFFF00}MP5!\n{BEBEBE}Confira os dados abaixo e confirme o pedido!\n\n{FA8072}Arma: {BEBEBE}Sub-Metralhadora\n{FA8072}Option:{BEBEBE} n/a\n{FA8072}Munição: {BEBEBE}10 Balas\n{FA8072}Valor Total: {FFFF00}1250,00","Comprar","Cancelar");

	}
	return 1;
}


public OnPlayerUpdate(playerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	if(dialogid == DIALOG_AUTENTIC){
	    if(!response) return ReasonMaxFalhas[playerid] = 2, SetTimerEx("r@ReasonKickP", 500,false,"i",playerid);

		if(strlen(inputtext) < 1) return ShowPlayerDialog(playerid, DIALOG_AUTENTIC, DIALOG_STYLE_PASSWORD, "Autenticar-se no Sistema!", "SENHA MIN 1 CARAC FDP.", "Autenticar", "Cancelar");
	    if(!strcmp(inputtext,DOF2_GetString(ObterConta(playerid),"Senha"),false)){

		r@LoadingDados(playerid);
		r@DistribuirDados(playerid);
		r@CarregarPos(playerid);

    	new arq[40];
    	format(arq, sizeof(arq), "Contas/%s.ini", pName(playerid));

		if(EPlayer[playerid][Cadastrado] == 0) {
		SpawnRandom(playerid);
		}
		else {
			SetSpawnInfo( playerid, 0, 199, EPlayer[playerid][UltimaPosX], EPlayer[playerid][UltimaPosY], EPlayer[playerid][UltimaPosZ], EPlayer[playerid][UltimoAngulo], 0, 0, 0, 0, 0, 0 );
		    //SetPlayerInterior(playerid, EPlayer[playerid][Interior]);
		    //SetPlayerVirtualWorld ( playerid, EPlayer[playerid][VirtualWorld] ) ;
			SpawnPlayer(playerid);
		}

		SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {FA8072}Conta verificada com sucesso. Seu personagem foi spawnado!");

		}
		else {
		 ErrouSenha[playerid]++;

		 format(StrAutentic, sizeof(StrAutentic), "{BEBEBE}Bem Vindo(a) ao {FFFF00}-------{BEBEBE}!\n\nA Conta {FA8072}%s {BEBEBE}foi encontrada no sistema, autentique sua senha abaixo para prosseguir.\n\n{FFD700}Jamais Passe sua senha para terceiros, nós não nos responsabilizamos por isso!", pName(playerid));
	     ShowPlayerDialog(playerid, DIALOG_AUTENTIC, DIALOG_STYLE_PASSWORD, "Autenticar-se no Sistema!", StrAutentic, "Autenticar", "Cancelar");
		 //ErrouDados(playerid);
		}
	}
	if(dialogid == DIALOG_SEMCONTA){
	    if(!response) return Kick(playerid);

		if(response) {

		if(strlen(inputtext) < 3){
			format(StrAutentic, sizeof(StrAutentic), "{BEBEBE}Bem Vindo(a) ao {FFFF00}-------{BEBEBE}!\n\nA Conta {FA8072}%s {BEBEBE}não foi encontrada no sistema, digite uma senha abaixo para prosseguir.\n\n{FFD700}Jamais Passe sua senha para terceiros, nós não nos responsabilizamos por isso!\n\n{FF0000}Preencha o campo abaixo com mais de 3 dígitos", pName(playerid));
			ShowPlayerDialog(playerid, DIALOG_SEMCONTA, DIALOG_STYLE_PASSWORD, "Autenticar-se no Sistema!", StrAutentic, "Autenticar", "Cancelar");
	    return 1;
 	 }


		    DOF2_CreateFile(ObterConta(playerid));

		    DOF2_SetString(ObterConta(playerid), "Senha" , inputtext);
		    DOF2_SetString(ObterConta(playerid), "Administrador", "Sem Cargo");
		    DOF2_SetString(ObterConta(playerid), "CidadeNatal", "Em Processo");


		    DOF2_SetInt(ObterConta(playerid), "Grana", 80);
		    DOF2_SetInt(ObterConta(playerid), "Level", 0);
		    DOF2_SetInt(ObterConta(playerid), "LvlWanted", 0);
		    DOF2_SetInt(ObterConta(playerid), "Cadastrado", 0); // - 0 pois será usada a verific. ao escolher Spawn Random!
		    //DOF2_SetInt(ObterConta(playerid), "Interior", 0);
		    //DOF2_SetInt(ObterConta(playerid), "VirtualWorld", 0);

		    DOF2_SetFloat(ObterConta(playerid), "UltimaPosX", 1079.0920);
		    DOF2_SetFloat(ObterConta(playerid), "UltimaPosY", 1761.0571);
		    DOF2_SetFloat(ObterConta(playerid), "UltimaPosZ", 13.3764);
			DOF2_SetFloat(ObterConta(playerid), "UltimoAngulo", 272.8557);

		    DOF2_SaveFile();

//		    CriarConquistas(playerid);

		    format(StrAutentic, sizeof(StrAutentic), "{BEBEBE}Bem Vindo(a) ao {FFFF00}-------{BEBEBE}!\n\nA Conta {FA8072}%s {BEBEBE}foi encontrada no sistema, autentique sua senha abaixo para prosseguir.\n\n{FFD700}Jamais Passe sua senha para terceiros, nós não nos responsabilizamos por isso!", pName(playerid));
		    ShowPlayerDialog(playerid, DIALOG_AUTENTIC, DIALOG_STYLE_PASSWORD, "Autenticar-se no Sistema!", StrAutentic, "Autenticar", "Cancelar");
		}
	}
	if(dialogid == DIALOG_PISTOL_SILENCE){
	    if(response){
			 new GetMoney = GetPlayerMoney(playerid);
			 if(GetMoney <= 450) return SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {BEBEBE}Você não possui dinheiro suficiente para finalizar essa compra!");
		     QualArmaCompro[playerid][0] = 1;

		     SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| A Arma foi encaminhada ao caixa, vá até ele para retirar elas e finalizar o pagamento!");

			 r@ConquistaWeapon(playerid);

			 ComproArma[playerid] = 1;
			 SetTimerEx("VarA0", 1000, 0, "i", playerid);
		}
		if(!response) {
 			ComproArma[playerid] = 1;
			SetTimerEx("VarA0", 1000, 0, "i", playerid);
	    }
	}
	else if(dialogid == DIALOG_PISTOL_EAGLE){
		if(response){
			 new GetMoney = GetPlayerMoney(playerid);
			 if(GetMoney <= 730) return SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {BEBEBE}Você não possui dinheiro suficiente para finalizar essa compra!");
		     //GivePlayerWeapon(playerid, 24, 10);
		     QualArmaCompro[playerid][1] = 1;
		     SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| A Arma foi encaminhada ao caixa, vá até ele para retirar elas e finalizar o pagamento!");

		     ComproArma[playerid] = 1;
			 SetTimerEx("VarA0", 1000, 0, "i", playerid);
		}
		if(!response) {
 			ComproArma[playerid] = 1;
			SetTimerEx("VarA0", 1000, 0, "i", playerid);
	    }
	}
	else if(dialogid == DIALOG_PISTOL_COLT45){
		if(response){
			 new GetMoney = GetPlayerMoney(playerid);
			 if(GetMoney <= 680) return SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {BEBEBE}Você não possui dinheiro suficiente para finalizar essa compra!");
		     //GivePlayerWeapon(playerid, 22, 10);
		     QualArmaCompro[playerid][2] = 1;
		     SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| A Arma foi encaminhada ao caixa, vá até ele para retirar elas e finalizar o pagamento!");
             GivePlayerMoney(playerid, GetMoney-680);

			 ComproArma[playerid] = 1;
			 SetTimerEx("VarA0", 1000, 0, "i", playerid);
		}
		if(!response) {
 			ComproArma[playerid] = 1;
			SetTimerEx("VarA0", 1000, 0, "i", playerid);
	    }
	}
	else if(dialogid == DIALOG_WEAPON_MP5){
		if(response){
			 new GetMoney = GetPlayerMoney(playerid);
			 if(GetMoney <= 1250) return SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {BEBEBE}Você não possui dinheiro suficiente para finalizar essa compra!");
		     //GivePlayerWeapon(playerid, 22, 10);
		     QualArmaCompro[playerid][3] = 1;
		     SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| A Arma foi encaminhada ao caixa, vá até ele para retirar elas e finalizar o pagamento!");

			 ComproArma[playerid] = 1;
			 SetTimerEx("VarA0", 1000, 0, "i", playerid);
		}
		if(!response) {
 			ComproArma[playerid] = 1;
			SetTimerEx("VarA0", 1000, 0, "i", playerid);
	    }
	}
	if(dialogid == DIALOG_MP5_SILENCE){
	    if(!response) return QualArmaCompro[playerid][0] = 0, QualArmaCompro[playerid][3] = 0, 0;

	    new GetMoney = GetPlayerMoney(playerid);

	    GivePlayerWeapon(playerid, 23, 10); //Pistola c/ Silenciador
	    QualArmaCompro[playerid][0] = 0;
	    GivePlayerMoney(playerid, GetMoney-450);

	    //

	    GivePlayerWeapon(playerid, 29, 10); // MP5
	    QualArmaCompro[playerid][3] = 0;
	    GivePlayerMoney(playerid, GetMoney-1250);
	}
	else if(dialogid == DIALOG_MP5_DESERT){
	    if(!response) return QualArmaCompro[playerid][3] = 0, QualArmaCompro[playerid][1] = 0, 0;

	    new GetMoney = GetPlayerMoney(playerid);

	     GivePlayerWeapon(playerid, 29, 10); // MP5
	     QualArmaCompro[playerid][3] = 0;
	     GivePlayerMoney(playerid, GetMoney-1250);

	     GivePlayerWeapon(playerid, 24, 10); // desert
         QualArmaCompro[playerid][1] = 0;
         GivePlayerMoney(playerid, GetMoney-730);
	}
	else if(dialogid == DIALOG_MP5_MM4){
	    if(!response) return QualArmaCompro[playerid][2] = 0, QualArmaCompro[playerid][3] = 0, 0;

	    new GetMoney = GetPlayerMoney(playerid);

	    GivePlayerWeapon(playerid, 22, 10);  // SILENCE 9MM
	    QualArmaCompro[playerid][2] = 0;
	    GivePlayerMoney(playerid, GetMoney-680);

	    GivePlayerWeapon(playerid, 29, 10); // MP5
	    QualArmaCompro[playerid][3] = 0;
	    GivePlayerMoney(playerid, GetMoney-1250);
	}
	return 1;
}

CMD:conquistas(playerid){
	new StrCONQQ[2000],
		TransfStr[2000]
		;
	format(TransfStr, sizeof(TransfStr), "    \t     {DAA520}Conquistas\n\n{BEBEBE}Conquista 01: {00BFFF}%s\n{BEBEBE}Conquista 02: {00BFFF}%s\n{BEBEBE}Conquista 03: {00BFFF}%s", DOF2_GetString(ObterConquistas(playerid), "Conquista01"),DOF2_GetString(ObterConquistas(playerid), "Conquista02"),DOF2_GetString(ObterConquistas(playerid), "Conquista03"));
	strcat(StrCONQQ, TransfStr);
	format(TransfStr, sizeof(TransfStr), "\n{BEBEBE}Conquista 04: {00BFFF}%s\n{BEBEBE}Conquista 05: {00BFFF}%s\n{BEBEBE}Conquista 06: {00BFFF}%s", DOF2_GetString(ObterConquistas(playerid), "Conquista04"),DOF2_GetString(ObterConquistas(playerid), "Conquista05"),DOF2_GetString(ObterConquistas(playerid), "Conquista06"));
	strcat(StrCONQQ, TransfStr);
	format(TransfStr, sizeof(TransfStr), "\n{BEBEBE}Conquista 07: {00BFFF}%s\n{BEBEBE}Conquista 08: {00BFFF}%s\n{BEBEBE}Conquista 09: {00BFFF}%s", DOF2_GetString(ObterConquistas(playerid), "Conquista07"),DOF2_GetString(ObterConquistas(playerid), "Conquista08"),DOF2_GetString(ObterConquistas(playerid), "Conquista09"));
	strcat(StrCONQQ, TransfStr);
	format(TransfStr, sizeof(TransfStr), "\n{BEBEBE}Conquista 10: {00BFFF}%s\n{BEBEBE}Conquista 11: {00BFFF}%s\n{BEBEBE}Conquista 12: {00BFFF}%s", DOF2_GetString(ObterConquistas(playerid), "Conquista10"),DOF2_GetString(ObterConquistas(playerid), "Conquista11"),DOF2_GetString(ObterConquistas(playerid), "Conquista12"));
	strcat(StrCONQQ, TransfStr);
	format(TransfStr, sizeof(TransfStr), "\n{BEBEBE}Conquista 13: {00BFFF}%s\n{BEBEBE}Conquista 14: {00BFFF}%s\n{BEBEBE}Conquista 15: {00BFFF}%s", DOF2_GetString(ObterConquistas(playerid), "Conquista13"),DOF2_GetString(ObterConquistas(playerid), "Conquista14"),DOF2_GetString(ObterConquistas(playerid), "Conquista15"));
	strcat(StrCONQQ, TransfStr);
	format(TransfStr, sizeof(TransfStr), "\n{BEBEBE}Conquista 16: {00BFFF}%s\n{BEBEBE}Conquista 17: {00BFFF}%s\n{BEBEBE}Conquista 18: {00BFFF}%s", DOF2_GetString(ObterConquistas(playerid), "Conquista16"),DOF2_GetString(ObterConquistas(playerid), "Conquista17"),DOF2_GetString(ObterConquistas(playerid), "Conquista18"));
	strcat(StrCONQQ, TransfStr);
	format(TransfStr, sizeof(TransfStr), "\n{BEBEBE}Conquista 19: {00BFFF}%s\n{BEBEBE}Conquista 20: {00BFFF}%s\n{BEBEBE}Conquista 21: {00BFFF}%s", DOF2_GetString(ObterConquistas(playerid), "Conquista19"),DOF2_GetString(ObterConquistas(playerid), "Conquista20"),DOF2_GetString(ObterConquistas(playerid), "Conquista21"));
	strcat(StrCONQQ, TransfStr);
	format(TransfStr, sizeof(TransfStr), "\n{BEBEBE}Conquista 22: {00BFFF}%s\n{BEBEBE}Conquista 23: {00BFFF}%s\n{BEBEBE}Conquista 24: {00BFFF}%s", DOF2_GetString(ObterConquistas(playerid), "Conquista22"),DOF2_GetString(ObterConquistas(playerid), "Conquista23"),DOF2_GetString(ObterConquistas(playerid), "Conquista24"));
	strcat(StrCONQQ, TransfStr);
	format(TransfStr, sizeof(TransfStr), "\n{BEBEBE}Conquista 25: {00BFFF}%s\n{BEBEBE}Conquista 26: {00BFFF}%s\n{BEBEBE}Conquista 27: {00BFFF}%s", DOF2_GetString(ObterConquistas(playerid), "Conquista25"),DOF2_GetString(ObterConquistas(playerid), "Conquista26"),DOF2_GetString(ObterConquistas(playerid), "Conquista27"));
	strcat(StrCONQQ, TransfStr);
	format(TransfStr, sizeof(TransfStr), "\n{BEBEBE}Conquista 28: {00BFFF}%s\n{BEBEBE}Conquista 29: {00BFFF}%s\n{BEBEBE}Conquista 30: {00BFFF}%s", DOF2_GetString(ObterConquistas(playerid), "Conquista28"),DOF2_GetString(ObterConquistas(playerid), "Conquista29"),DOF2_GetString(ObterConquistas(playerid), "Conquista30"));
	strcat(StrCONQQ, TransfStr);
	ShowPlayerDialog(playerid, 100, DIALOG_STYLE_MSGBOX, "Conquistas!", StrCONQQ, "Fechar", "Proximo");
	return 1;
}

CMD:gmx() return
	SendRconCommand("gmx");


CMD:grana(playerid) return
	GivePlayerMoney(playerid, 10000);


CMD:criarveiculo( playerid, params[] ) {
	new c1, c2;
	if( sscanf(params, "ddd", id, c1, c2) ) {
		return SendClientMessage( playerid, -1, "{BEBEBE}| {FF0000}ERRO{BEBEBE} | {FA8072}O Comando foi digita incorretamente. Modo Correto: /CriarVeiculo [ID] [COR01] [COR02]" );
	}

	GetPlayerPos( playerid, _x, _y, _z );

	if( criarc[playerid] ) DestroyVehicle( criarc[playerid] );
	criarc[playerid] = AddStaticVehicleEx(id, _x, _y, _z, 0.0, c1, c2, 0);
	SetVehicleNumberPlate(criarc[playerid], "SA-MP");
	PutPlayerInVehicle( playerid, criarc[playerid], 0 );
	if( GetPlayerInterior(playerid) ) LinkVehicleToInterior( criarc[playerid], GetPlayerInterior(playerid) );

	format( String, sizeof(String), "{FA8072}Equipe ####\n{ff8300}%s", pName(playerid) );
	criarcText[playerid] = Create3DTextLabel( String, 0xFF0000AA, 0.0, 0.0, 0.0, 10.0, 0, 1 );
    Attach3DTextLabelToVehicle( criarcText[playerid], criarc[playerid], 0.0, 0.0, 2.0);
	return 1;
}

stock ObterConquistas(playerid){
	new ConquistasVAR[40];
	format(ConquistasVAR, sizeof(ConquistasVAR), "Conquistas/%s.ini", pName(playerid));
	return ConquistasVAR;
}

stock ObterConta(playerid) {
    new arq[40];
    format(arq, sizeof(arq), "Contas/%s.ini", pName(playerid));
    return arq;
}

stock pName(playerid) {
    new name[24];
    GetPlayerName(playerid, name, sizeof(name));
    return name;
}

r@LoadingDados(playerid) {
	EPlayer[playerid][Grana] = DOF2_GetInt(ObterConta(playerid), "Grana");
	EPlayer[playerid][Level] = DOF2_GetInt(ObterConta(playerid), "Level");
	EPlayer[playerid][Exp] = DOF2_GetInt(ObterConta(playerid), "Exp");
	EPlayer[playerid][LvlWanted] = DOF2_GetInt(ObterConta(playerid), "LvlWanted");
	EPlayer[playerid][Cadastrado] = DOF2_GetInt(ObterConta(playerid), "Cadastrado");
	//EPlayer[playerid][Interior] = DOF2_GetInt(ObterConta(playerid), "Interior");
	//EPlayer[playerid][VirtualWorld] = DOF2_GetInt(ObterConta(playerid), "VirtualWorld");

	format(EPlayer[playerid][CidadeNatal], MAX_PLAYER_NAME, DOF2_GetString(ObterConta(playerid), "CidadeNatal"));
	return 1;
}

r@DistribuirDados(playerid) {

	GivePlayerMoney(playerid, EPlayer[playerid][Grana]);
	SetPlayerScore(playerid, EPlayer[playerid][Level]);
	SetPlayerWantedLevel(playerid, EPlayer[playerid][LvlWanted]);

	return 1;
}

r@SalvarDados(playerid) {
 	DOF2_SetInt(ObterConta(playerid), "Grana", GetPlayerMoney(playerid));
	DOF2_SetInt(ObterConta(playerid), "Level", GetPlayerScore(playerid));
	DOF2_SetInt(ObterConta(playerid), "Exp", EPlayer[playerid][Exp]);
	DOF2_SetInt(ObterConta(playerid), "LvlWanted", GetPlayerWantedLevel(playerid));
	DOF2_SetInt(ObterConta(playerid), "Cadastrado", 1);
	//DOF2_SetInt(ObterConta(playerid), "Interior", GetPlayerInterior(playerid));
	//DOF2_SetInt(ObterConta(playerid), "VirtualWorld", GetPlayerVirtualWorld(playerid));

	DOF2_SaveFile();
	return 1;
}

r@SalvarPos(playerid) {
	GetPlayerPos(playerid, EPlayer[playerid][UltimaPosX], EPlayer[playerid][UltimaPosY], EPlayer[playerid][UltimaPosZ]);
	GetPlayerFacingAngle(playerid, EPlayer[playerid][UltimoAngulo]);

 	DOF2_SetFloat(ObterConta(playerid), "UltimaPosX", EPlayer[playerid][UltimaPosX]);
    DOF2_SetFloat(ObterConta(playerid), "UltimaPosY", EPlayer[playerid][UltimaPosY]);
    DOF2_SetFloat(ObterConta(playerid), "UltimaPosZ", EPlayer[playerid][UltimaPosZ]);
    DOF2_SetFloat(ObterConta(playerid), "UltimoAngulo", EPlayer[playerid][UltimoAngulo]);

	DOF2_SaveFile();
	return 1;
}

r@CarregarPos(playerid) {
	EPlayer[playerid][UltimaPosX] = DOF2_GetFloat(ObterConta(playerid), "UltimaPosX");
	EPlayer[playerid][UltimaPosY] = DOF2_GetFloat(ObterConta(playerid), "UltimaPosY");
	EPlayer[playerid][UltimaPosZ] = DOF2_GetFloat(ObterConta(playerid), "UltimaPosZ");
	EPlayer[playerid][UltimoAngulo] = DOF2_GetFloat(ObterConta(playerid), "UltimoAngulo");

	return 1;
}


ErrouDados(playerid) {
	if(ErrouSenha[playerid] >= 3) return ReasonMaxFalhas[playerid] = 1, SetTimerEx("r@ReasonKickP", 1000,false,"i",playerid);
	format(StrAutentic, sizeof(StrAutentic), "{BEBEBE}Bem Vindo(a) ao {FFFF00}-------{BEBEBE}!\n\nA Conta {FA8072}%s {BEBEBE}foi encontrada no sistema, digite a senha {FF0000}corretamente {BEBEBE}abaixo.\n\n{FFD700}Jamais Passe sua senha para terceiros, nós não nos responsabilizamos por isso!\n\n{FF0000}Autenticação Falhou!", pName(playerid));
	ShowPlayerDialog(playerid, DIALOG_AUTENTIC, DIALOG_STYLE_PASSWORD, "Autenticar-se no Sistema!", StrAutentic, "Autenticar", "Cancelar");
	return 1;
}

forward r@ReasonKickP(playerid);
public r@ReasonKickP(playerid) {
	if(ReasonMaxFalhas[playerid] == 1) {
		ShowPlayerDialog(playerid, DIALOG_KICKMAXERRORS, DIALOG_STYLE_MSGBOX, "{BEBEBE}Proteção á {FFFF00}Conta{BEBEBE}!", "{BEBEBE}Você foi {FF0000}desconectado {BEBEBE}do servidor por tentar muitas vezes á senha da conta e erra-la.\nEssa medida evita roubos ou tentativas de fakers. Caso isso se repita puniçõe serão aderidas!", "Encerrar", "");

		SetTimerEx("r@KickPlayer", 500,false,"i",playerid);
	}
	else if(ReasonMaxFalhas[playerid] == 2) {
	    ShowPlayerDialog(playerid, DIALOG_KICKMAXERRORS, DIALOG_STYLE_MSGBOX, "{BEBEBE}Ação Fechar!", "{BEBEBE}Você foi {FF0000}desconectado {BEBEBE}do servidor por realizar a saída manual.\nEssa mensagem foi enviada para lhe informar sobre sua ação.\n\n{FFFF00}Volte Sempre!", "Encerrar", "");

        SetTimerEx("r@KickPlayer", 500,false,"i",playerid);
	}
	else if(ReasonMaxFalhas[playerid] == 3){
	     ShowPlayerDialog(playerid, DIALOG_KICKMAXERRORS, DIALOG_STYLE_MSGBOX, "{BEBEBE}Nick Proibido!", "{BEBEBE}Você foi {FF0000}desconectado {BEBEBE}do servidor por usar um nome proibido.\nEsse nick é proibido para evitar roubos á jogadores, haverá {FF0000}punição{BEBEBE} em caso de persistencia.\n\n{FFFF00}Volte Sempre!", "Encerrar", "");

        SetTimerEx("r@KickPlayer", 500,false,"i",playerid);
	}
	return 1;
}

forward r@KickPlayer(playerid);
public r@KickPlayer(playerid) {
	Kick(playerid);
	return 1;
}

stock SpawnRandom(playerid) {
    if(!strcmp(LocalSpawn[random(sizeof(LocalSpawn))], "Dillimore")) {
		/* - Anotações

				* Posição em local aberto, próximo ao posto de Gasolina e DP de Dillimore.
				* Dillimore está localizada logo ao norte de Los Santos
				* População Aproximada: 3.265
				* Dillimore é considerada uma das cidades mais bonitas de San Andreas
				* Contém um Pay 'n' Spray, Lanchonete, Barbearia, Delegacia e Posto de Gasolina!
		*/
		SetSpawnInfo( playerid, 0, 154, 665.3912,-619.0103, 16.3359, 1.1706, 0, 0, 0, 0, 0, 0 );
  		SpawnPlayer(playerid);

		GameTextForPlayer(playerid, "~b~Dillimore", 2000, 1);

		ShowPlayerDialog(playerid, DIALOG_SPAWN-DILLI, DIALOG_STYLE_MSGBOX, "{BEBEBE}Cidade Natal!", "{BEBEBE}A sua Cidade Natal é {FFFF00}Dillimore!\n\n\
																									  {BEBEBE}A População de Dillimore é de aproximadamente {FFFF00}3.265 {BEBEBE}Habiatentes!\n\
																									  {BEBEBE}É uma cidade pequena, na qual você nasceu e será sua {FA8072}Cidade Natal {BEBEBE}até o fim da sua vida.\n\n\
																									  {FFFF00}Boa Sorte!", "Fechar","");

		DOF2_SetString(ObterConta(playerid), "CidadeNatal", "Dillimore");
		DOF2_SaveFile();
	}
    else if(!strcmp(LocalSpawn[random(sizeof(LocalSpawn))], "PalominoCreek")) {
		/* - Anotações

				* Posição Entre dois muros, perto da DP de Palomino Creek!
				* Palomino Creek é uma vila localizada na parte nordeste de Red County.
				* Lá se encontram um banco, um Pizza Stack, uma Ammu-Nation e o Hanky Panky Point!
				* População aproximada: 6.836
		*/
		SetSpawnInfo( playerid, 0, 154,2308.5635,70.4063,26.4844,89.5940, 0, 0, 0, 0, 0, 0 );
		SpawnPlayer(playerid);

		GameTextForPlayer(playerid, "~b~Palomino", 2000, 1);

		ShowPlayerDialog(playerid, DIALOG_SPAWN-DILLI, DIALOG_STYLE_MSGBOX, "{BEBEBE}Cidade Natal!", "{BEBEBE}A sua Cidade Natal é {FFFF00}Palomino Creek!\n\n\
																									  {BEBEBE}A População de Palomino Creek é de aproximadamente {FFFF00}6.836 {BEBEBE}Habiatentes!\n\
																									  {BEBEBE}É uma vila localizada ao nordeste de Red Country na qual você nasceu e será sua {FA8072}Cidade Natal {BEBEBE}até o fim da sua vida.\n\n\
																									  {FFFF00}Boa Sorte!", "Fechar","");

		DOF2_SetString(ObterConta(playerid), "CidadeNatal", "Palomino Creek");
		DOF2_SaveFile();
    }
    else if(!strcmp(LocalSpawn[random(sizeof(LocalSpawn))], "Blueberry")) {
        /* - Anotações

				* Posição da Cerca, próximo a lixeiras.
        		* Blueberry é uma pequena cidade ladeada por uma grande fazenda (Blueberry Acres). A cidade possui um Pizza Stack e um Ammu-Nation.
				* População aproximada: 3.309.
		*/
		SetSpawnInfo( playerid, 0, 154,261.9953,-138.4277,1.5781,92.9449, 0, 0, 0, 0, 0, 0 );
		SpawnPlayer(playerid);

		GameTextForPlayer(playerid, "~b~Blueberry", 2000, 1);

		ShowPlayerDialog(playerid, DIALOG_SPAWN-DILLI, DIALOG_STYLE_MSGBOX, "{BEBEBE}Cidade Natal!", "{BEBEBE}A sua Cidade Natal é {FFFF00}Blueberry!\n\n\
																									  {BEBEBE}A População de Blueberry é de aproximadamente {FFFF00}3.309 {BEBEBE}Habiatentes!\n\
																									  {BEBEBE}É uma pequena cidade ladeada por uma grande fazenda na qual você nasceu e será sua {FA8072}Cidade Natal {BEBEBE}até o fim da sua vida.\n\n\
																									  {FFFF00}Boa Sorte!", "Fechar","");

		DOF2_SetString(ObterConta(playerid), "CidadeNatal", "Blueberry");
		DOF2_SaveFile();
    }
    else if(!strcmp(LocalSpawn[random(sizeof(LocalSpawn))], "AngelPine")) {
		/* - Anotações

		        * Posição próximo á Caixas e Lixos e Ammu-Nation
		        * Angel Pine está localizada ao sul do Monte Chiliad.
		        * Possui um Ammu-Nation, uma delegacia, um pequeno hospital, um Cluckin' Bell e um estacionamento de trailers
		        * Angel Pine é a cidade com o maior número de fazendas em San Andreas
				* População aproximada de Angel Pine: 7.539
		*/
		SetSpawnInfo( playerid, 0, 154,-2177.6023,-2427.3831,30.6250,231.9010, 0, 0, 0, 0, 0, 0 );
		SpawnPlayer(playerid);

		GameTextForPlayer(playerid, "~b~Angel Pine", 2000, 1);

   		ShowPlayerDialog(playerid, DIALOG_SPAWN-DILLI, DIALOG_STYLE_MSGBOX, "{BEBEBE}Cidade Natal!", "{BEBEBE}A sua Cidade Natal é {FFFF00}Angel Pine!\n\n\
																									  {BEBEBE}A População de Angel Pine é de aproximadamente {FFFF00}7.539 {BEBEBE}Habiatentes!\n\
																									  {BEBEBE}É uma cidade localizada proximo ao Monte Chilliad na qual você nasceu e será sua {FA8072}Cidade Natal {BEBEBE}até o fim da sua vida.\n\n\
																									  {FFFF00}Boa Sorte!", "Fechar","");

		DOF2_SetString(ObterConta(playerid), "CidadeNatal", "Angel Pine");
		DOF2_SaveFile();
    }
    else if(!strcmp(LocalSpawn[random(sizeof(LocalSpawn))], "FortCarson")) {
        /* - Anotações

            	* Posição entre dois muros e vista para cercas fodidas.
            	* A cidade possui um 24-7, uma delegacia, um pequeno hospital, um Pay 'n' Spray, um Cluckin' Bell, uma Ammu-Nation e um Bar
                * A noroeste da cidade encontra-se a "Big Ear", uma grande antena de satélite que dizem transmitir radiação para a população próxima
                * Ao sudoeste da cidade tem uma estrada que leva até a autoestrada que liga San Fierro e Las Venturas!
		*/
		SetSpawnInfo( playerid, 0, 154, -159.1721,1163.0107,19.7422,185.0957, 0, 0, 0, 0, 0, 0 );
		SpawnPlayer(playerid);

		GameTextForPlayer(playerid, "~b~Fort Carson", 2000, 1);

		ShowPlayerDialog(playerid, DIALOG_SPAWN-DILLI, DIALOG_STYLE_MSGBOX, "{BEBEBE}Cidade Natal!", "{BEBEBE}A sua Cidade Natal é {FFFF00}Fort Carson!\n\n\
																									  {BEBEBE}A População de Fort Carson é de aproximadamente {FFFF00}2.528 {BEBEBE}Habiatentes!\n\
																									  {BEBEBE}É uma cidade localizada próximo a uma antena com muitas lendas na qual você nasceu e será sua {FA8072}Cidade Natal {BEBEBE}até o fim da sua vida.\n\n\
																									  {FFFF00}Boa Sorte!", "Fechar","");

		DOF2_SetString(ObterConta(playerid), "CidadeNatal", "Fort Carson");
		DOF2_SaveFile();
    }
    else if(!strcmp(LocalSpawn[random(sizeof(LocalSpawn))], "Montgomery")) {
		/* - Anotações

				* Posição na entrada da Vila, próximo a muros
				* Possui um pequeno hospital, Crippen Memorial, e uma casa de apostas em corridas de cavalos
				* População aproximada: 6.623
				* Montgomery na vida real chama-se Three Forks.
		*/
		SetSpawnInfo( playerid, 0, 154, 1348.9589,207.3967,19.5547,335.2722, 0, 0, 0, 0, 0, 0 );
		SpawnPlayer(playerid);

		GameTextForPlayer(playerid, "~b~Montgomery", 2000, 1);


		ShowPlayerDialog(playerid, DIALOG_SPAWN-DILLI, DIALOG_STYLE_MSGBOX, "{BEBEBE}Cidade Natal!", "{BEBEBE}A sua Cidade Natal é {FFFF00}Montgomery!\n\n\
																									  {BEBEBE}A População de Montgomery é de aproximadamente {FFFF00}6.623 {BEBEBE}Habiatentes!\n\
																									  {BEBEBE}É uma cidade que foi baseada na cidade de {FFFF00}Three Forks {BEBEBE}na qual você nasceu e será sua {FA8072}Cidade Natal {BEBEBE}até o fim da sua vida.\n\n\
																									  {FFFF00}Boa Sorte!", "Fechar","");
		DOF2_SetString(ObterConta(playerid), "CidadeNatal", "Montgomery");
		DOF2_SaveFile();
    }
	return 1;
}

stock Estrelas(playerid) return GetPlayerWantedLevel(playerid);


r@CarregarVehPublicos(){
	CarsPublic[0] = AddStaticVehicle(475,999.6005,-1043.3523,30.5670,270.2757,1,60000);
	CarsPublic[1] = AddStaticVehicle(400,1301.1741,324.8104,19.1343,65.9683,1,60000);
	CarsPublic[2] = AddStaticVehicle(401,-177.5457,1219.9767,19.4693,269.7057,1,60000);
	CarsPublic[3] = AddStaticVehicle(402,-136.0282,1175.8167,19.4771,1.0650,1,60000);
	CarsPublic[4] = AddStaticVehicle(415,-1987.8926,305.2485,34.9027,89.1461,1,60000);
	CarsPublic[5] = AddStaticVehicle(421,-1987.8582,164.7788,27.2661,359.0646,1,60000);
	CarsPublic[6] = AddStaticVehicle(436,-2064.0129,-85.1373,34.8911,0.4080,1,60000);
	CarsPublic[7] = AddStaticVehicle(439,-1734.4329,-109.9843,3.2818,136.8653,1,60000);
	CarsPublic[8] = AddStaticVehicle(445,406.1383,-1438.4972,32.0257,216.7599,1,60000);
	CarsPublic[9] = AddStaticVehicle(458,1005.8078,-1367.7750,13.0419,1.7135,1,60000);
	CarsPublic[10] = AddStaticVehicle(461,1065.1233,-1179.2733,22.0812,1.6954,1,60000);

	return 1;
}


forward VarA0(playerid);
public VarA0(playerid){
    ComproArma[playerid] = 0;
    return 1;
}

stock CarregarAmmuLS(){
	CreateObject(346, 309.98, -161.68, 999.62,   -90.00, 0.00, 262.96);
	CreateObject(348, 310.01, -162.69, 999.60,   -90.00, 0.00, 270.00);
	CreateObject(347, 310.64, -163.15, 999.62,   -90.00, 0.00, -5.00);
	CreateObject(349, 311.56, -162.50, 999.67,   -90.00, -360.00, 76.00);
	CreateObject(2038, 311.43, -162.07, 999.56,   90.00, 0.00, 0.00);
	CreateObject(353, 310.81, -161.44, 999.60,   90.00, 0.00, -4.00);
	CreateObject(2038, 310.54, -161.51, 999.58,   90.00, 0.00, 0.00);
	CreateObject(2038, 311.43, -162.07, 999.60,   90.00, 0.00, 0.00);
	CreateObject(355, 314.85, -163.16, 999.63,   -90.00, 0.00, 0.00);
	CreateObject(2038, 322.45, -177.21, 999.56,   90.00, 0.00, 0.00);
	CreateObject(2038, 315.36, -163.06, 999.56,   90.00, 0.00, 0.00);
	CreateObject(356, 315.83, -162.60, 999.60,   -90.00, 0.00, 90.00);
	CreateObject(2038, 315.77, -162.10, 999.54,   90.00, 0.00, 0.00);
	CreateObject(358, 314.77, -161.49, 999.62,   90.00, 0.00, 0.00);
	CreateObject(2038, 314.72, -161.55, 999.58,   90.00, 0.00, 0.00);
	CreateObject(336, 314.30, -162.01, 999.60,   90.00, 0.00, 0.00);
	CreateObject(2038, 314.32, -162.15, 999.56,   90.00, 0.00, 0.00);
	CreateObject(359, 311.29, -166.83, 999.60,   90.00, 0.00, 0.00);
	CreateObject(342, 318.93, -163.00, 999.75,   90.00, 0.00, 0.00);
	CreateObject(342, 319.24, -163.40, 999.75,   90.00, 0.00, 69.00);
	CreateObject(342, 319.24, -163.50, 999.75,   90.00, 0.00, 69.00);
	CreateObject(342, 319.24, -163.62, 999.75,   90.00, 0.00, 69.00);
	CreateObject(342, 319.08, -163.38, 999.75,   90.00, 0.00, 69.00);
	CreateObject(342, 319.08, -163.50, 999.75,   90.00, 0.00, 69.00);
	CreateObject(342, 319.08, -163.60, 999.75,   90.00, 0.00, 69.00);
	CreateObject(342, 318.92, -163.60, 999.75,   90.00, 0.00, 69.00);
	CreateObject(342, 318.92, -163.48, 999.75,   90.00, 0.00, 69.00);
	CreateObject(342, 318.92, -163.36, 999.75,   90.00, 0.00, 69.00);
	CreateObject(344, 318.85, -161.39, 999.96,   0.00, 0.00, 0.00);
	CreateObject(344, 319.25, -161.77, 999.96,   0.00, 0.00, 0.00);
	CreateObject(344, 319.24, -161.89, 999.96,   0.00, 0.00, 0.00);
	CreateObject(344, 319.25, -161.99, 999.96,   0.00, 0.00, 0.00);
	CreateObject(344, 319.11, -161.98, 999.96,   0.00, 0.00, 0.00);
	CreateObject(344, 319.14, -161.86, 999.96,   0.00, 0.00, 0.00);
	CreateObject(344, 319.13, -161.76, 999.96,   0.00, 0.00, 0.00);
	CreateObject(344, 318.99, -161.75, 999.96,   0.00, 0.00, 0.00);
	CreateObject(344, 318.98, -161.89, 999.96,   0.00, 0.00, 0.00);
	CreateObject(344, 318.97, -161.99, 999.96,   0.00, 0.00, 0.00);
	CreateObject(373, 318.94, -160.12, 1000.23,   -30.00, -55.00, 186.00);
}

r@ConquistaWeapon(playerid){
	if(strcmp("Atirador Mirim I", DOF2_GetString(ObterConquistas(playerid), "Conquista02"))) {
		SendClientMessage(playerid, -1, "");
		SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {BEBEBE}Você acaba de adquirir uma conquista!");
		SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {BEBEBE}Você poderá comprar armas em qualquer Ammu-Nation local!");
		SendClientMessage(playerid, -1, "{BEBEBE}|{FFD700} INFO {BEBEBE}| {FA8072}Conquista Desbloqueada! {1E90FF}- {FFD700}Atirador Mirim I");

		DOF2_SetString(ObterConquistas(playerid), "Conquista02", "Atirador Mirim I");
		DOF2_SaveFile();
	}
}
#include "sistema_pontos_cqdlulis.h"
#include "keyboard.h"
#include "display.h"
#include "tempo_wifi.h"

void limpar_id(char id_teclado[5]); // manuseio da cadeia de char
void painel_principal(); // função para o painel principal que é escrito várias vezes

void app_main(){

  tempo_init();

  char id_teclado[5];
  int indice_id = 0;

  limpar_id(id_teclado);

  keypad_init();

  i2c_init();
  lcd_init();

  arena arena1 = setArena(2048);

  // Tabela de criação de usuários
  criarusuario(&arena1, "Camila", "15:45:00", "18:00:00", "2004");
  criarusuario(&arena1, "Luiz", "11:00:00", "18:00:00", "2005");
  criarusuario(&arena1, "Bezerra", "11:00:00", "18:00:00", "1000");

  painel_principal(id_teclado);

  while (1) {
    char key = keypad_read();
  
    if(indice_id == 4){
      if(key != '\0'){
        if(key == 'D'){
          for(int i = 0; i < id; i++){
            if(strcmp(usuario[i]->credencial, id_teclado) == 0){
              limpar_id(id_teclado);
              indice_id = 0;
              lcd_clear();
              lcd_set_cursor(0,0);
              if(usuario[i]->veiohoje){
                lcd_print("Ate a proxima!");
              }
              else{
                chegou(usuario[i]);
                lcd_print("Bem vindo(a)");
              }
              lcd_set_cursor(0,1);
              lcd_print(usuario[i]->username);
              break;
            }
          }
        }
        else if(key == 'B'){
          for(int i = 0; i < id; i++){
            if(strcmp(usuario[i]->credencial, id_teclado) == 0){

              char faltas_print[16];

              limpar_id(id_teclado);
              indice_id = 0;
              lcd_clear();
              lcd_set_cursor(0,0);
              lcd_print("Faltas: ");
              lcd_set_cursor(0,1);

              sprintf(faltas_print, "%d", usuario[i]->faltas);

              lcd_print(faltas_print);

              break;
            }
          }
        }

        else if(key == 'C'){
          for(int i = 0; i < id; i++){
            if(strcmp(usuario[i]->credencial, id_teclado) == 0){

              char atraso_print[16];

              limpar_id(id_teclado);
              indice_id = 0;
              lcd_clear();
              lcd_set_cursor(0,0);
              lcd_print("Atrasos: ");
              lcd_set_cursor(0,1);

              sprintf(atraso_print, "%d", usuario[i]->atrasos);

              lcd_print(atraso_print);

              break;
            }
          }
        }

        else if(key == 'A'){
          if(strcmp(id_teclado, "0000") == 0){
            for(int i = 0; i < id; i++){
              fim_do_dia(usuario[i]);
            }
            lcd_clear();
            lcd_set_cursor(0,0);
            lcd_print("Fim do");
            lcd_set_cursor(0,1);
            lcd_print("Expediente!");
          }
        }
        else{
          limpar_id(id_teclado);
          indice_id = 0;
          painel_principal(id_teclado);
        }
      }
    }
    else{
      if (key == '1' ||key == '2' || key == '3' ||
      key == '4' ||key == '5' || key == '6' ||
      key == '7' ||key == '8' || key == '9' ||
      key == '0'){

        id_teclado[indice_id] = key;
        indice_id++;
        painel_principal(id_teclado);

      }
    }
  }

  freeArena(&arena1);

}

void limpar_id(char id_teclado[5]){
  id_teclado[0] = '0';
  for(int i = 1; i <5; i++){
    id_teclado[i] = '\0';
  }
}

void painel_principal(char id_teclado[5]){
  lcd_clear();
  lcd_set_cursor(0,0);
  lcd_print("Digite seu ID:");
  lcd_set_cursor(0,1);
  lcd_print(id_teclado);
}
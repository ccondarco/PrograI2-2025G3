#include <iostream>
#include "libreriaDePrueba.h"
using namespace std;

int main() {
    system("chcp 65001");
    system("cls");
    
    string archivoProducto = "Productos_02.bin";
    vector<string> metodos_pago = {"Efectivo", "Tarjeta", "QR"};
    int opcion;
    do {
        system("cls");
        cout << "MAIN MENU\n";
        cout << "1.PRODUCTOS\n";
        cout << "2.FACTURAS Y VENTAS\n";
        cout << "3.C LIENTES Y EMPLEADOS\n";
        cout << "0. SALIR\n";
        cout << "-->";
        cin >> opcion;
        if (opcion==1) {
            menuABM_Productos(archivoProducto);
        } else if (opcion==2){
            menu_principal("Ventas", "Fact", archivoProducto, metodos_pago);
        } else if(opcion==3) {
            Menu();
        }else if (opcion==0) {
            cout << "Saliendo...";
        }
    } while (opcion!=0);
    
    return 0;
}
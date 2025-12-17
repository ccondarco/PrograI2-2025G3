#include <iostream>
#include <fstream>
#include <vector>
#include "lib_ana.h"

using namespace std;

int main()
{
    system("chcp 65001");
    system("cls");

    string archivo_facturas = "Facturas.bin";
    string archivo_ventas = "Ventas.bin";
    string archivo_productos = "Productos.bin";
    vector<string> metodos_pago = {"Efectivo", "Tarjeta", "QR"};

    crear_productos_prueba(archivo_productos);

    menu_principal(archivo_ventas, archivo_facturas, archivo_productos, metodos_pago);

    return 0;
}


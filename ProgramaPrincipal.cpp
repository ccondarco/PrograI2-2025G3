#include <iostream>
#include <fstream>
#include <vector>
#include "lib_ana.h"

using namespace std;



void menu_facturacion(string nombre_archivo_ventas, string nombre_archivo_facturas, vector<string> metodos_pago);
void adicionar_venta(string nombre_archivo_ventas);
void modificar_venta(string nombre_archivo_ventas);
void eliminar_venta(string nombre_archivo_ventas);
int generar_numero_factura(string nombre_archivo_facturas);
void crear_factura(string nombre_archivo_facturas, string nombre_archivo_ventas, vector<string> metodos_pago);
void anular_factura(string nombre_archivo_facturas);
void listar_ventas_pendientes(string nombre_archivo_ventas);

int main()
{
    system("chcp 65001");
    system("cls");

    string archivo_facturas = "Facturas.bin";
    string archivo_ventas = "Ventas.bin";
    string archivo_productos = "Productos.bin";
    vector<string> metodos_pago = {"Efectivo", "Tarjeta", "QR"};

    menu_facturacion(archivo_facturas, archivo_ventas, metodos_pago);

    return 0;
}
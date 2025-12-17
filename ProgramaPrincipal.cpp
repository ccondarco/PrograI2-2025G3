#include <iostream>
#include <fstream>
#include <vector>
#include "lib_ana.h"

using namespace std;

void crear_productos_prueba(string nombre_archivo_productos);
void menu_facturacion(string nombre_archivo_ventas, string nombre_archivo_facturas, vector<string> metodos_pago, string nombre_archivo_productos);
void adicionar_venta(string nombre_archivo_ventas);
void modificar_venta(string nombre_archivo_ventas);
void eliminar_venta(string nombre_archivo_ventas);
int generar_numero_factura(string nombre_archivo_facturas);
void crear_factura(string nombre_archivo_facturas, string nombre_archivo_ventas, vector<string> metodos_pago, string nombre_archivo_productos);
void anular_factura(string nombre_archivo_facturas, string nombre_archivo_ventas, string nombre_archivo_productos);
void listar_ventas_pendientes(string nombre_archivo_ventas);
void listar_facturas(string nombre_archivo_facturas, vector<string> metodos_pago, string nombre_archivo_ventas);
void mostrar_detalle_factura(string nombre_archivo_facturas, string nombre_archivo_ventas);
void menu_mostrar_detalle_factura(string nombre_archivo_ventas, string nombre_archivo_facturas);
void descontar_stock(int codigoProducto, int cantidad_vendida, string nombre_archivo_productos);
void sumar_stock(int codigoProducto, int cantidad_devuelta, string nombre_archivo_productos);
void reporte_ventas_cliente(string nombre_archivo_ventas);
void reporte_ventas_diarias(string nombre_archivo_ventas);
void reporte_diario_productos_disponibles(string nombre_archivo_productos);
void alerta_stock_producto(int codigoProducto, string nombre_archivo_productos, int limite);
void menu_reportes(string nombre_archivo_ventas, string nombre_archivo_productos);
void reporte_temporada_alta(string nombre_archivo_ventas);
void menu_principal(string nombre_archivo_ventas, string nombre_archivo_facturas, string nombre_archivo_productos, vector<string> metodos_pago);

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

#include <iostream>

using namespace std;

struct Fecha
{
    int mes;
    int dia;
    int anio;
};

struct DatosCliente
{
    char CI_Cliente[30];
    char nombre[30];
    char apellido[30];
    Fecha fechaNacimiento;
    int telefono;
    char correo[30];
};

struct DatosEmpleado
{
    char CI_Empleado[10];
    char nombre[30];
    char apellido[30];
    int rango;
};

struct DatosProducto
{
    int codigo;
    char modelo[30];
    int categoria;
    float precioVenta;
    int stock;
    bool eliminado;
};

struct ventaProducto
{
    int codigoProducto;
    char CI_Cliente[10];
    char CI_Empleado[10];
    Fecha fechaDeVenta;
    int cantidad;
    float precioUnitario;
    bool eliminada;
    bool facturada;
    int numeroFactura;
};

struct Reparaciones
{
    char CI_cliente[10];
    int codigoProducto;
    int numeroFactura;
    char descripcion[100];
    float precioReparacion;
    Fecha fechaReparacion;
};

struct datosFactura
{
    int numeroFactura;
    char CI_cliente[10];
    Fecha fecha_emision_factura;
    int metodo_pago;
    bool anulada;
};

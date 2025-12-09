#include <iostream>
#include <fstream>

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
    bool eliminado;
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
};

int generar_numero_factura(string nombre_archivo_facturas)
{
    datosFactura factura_actual;
    ifstream archivo_facturas;
    int contador = 0;

    archivo_facturas.open(nombre_archivo_facturas, ios::binary);

    if (archivo_facturas.good())
    {
        while (archivo_facturas.read((char*)&factura_actual, sizeof(datosFactura)))
        {
            contador = contador + 1;
        }
        return contador;
    }
    else
    {
        return 1;
    }

    archivo_facturas.close();
}

void crear_factura(string nombre_archivo_facturas, string nombre_archivo_ventas, vector<string> metodos_pago)
{
    datosFactura factura_nueva;
    ofstream archivo_facturas;
    int numero_factura;

    archivo_facturas.open(nombre_archivo_facturas, ios::binary | ios::app);

    if (archivo_facturas.good())
    {
        cin.ignore();
        cout << "Ingresar CI Cliente: ";
        cin.getline(factura_nueva.CI_cliente, 10);

        cout << "Ingresar fecha de emisión:" << endl;
        cout << "Día: "; cin >> factura_nueva.fecha_emision_factura.dia;
        cout << "Mes: "; cin >> factura_nueva.fecha_emision_factura.mes;
        cout << "Año: "; cin >> factura_nueva.fecha_emision_factura.anio;

        cout << "\tMÉTODOS DE PAGO" << endl;
        for (int i = 0; i < metodos_pago.size(); i++)
        {
            cout << "\t" << i + 1 << " " << metodos_pago[i] << endl;
        }
        cout << "Ingresar método de pago: ";
        cin >> factura_nueva.metodo_pago;

        archivo_facturas.write((char*)&factura_nueva, sizeof(datosFactura));
        archivo_facturas.close();

        numero_factura = generar_numero_factura(nombre_archivo_facturas);

        cout << "Factura creada con número: " << numero_factura << endl;
        system("pause");
    }
    else
    {
        cout << "No se pudo abrir archivo de facturas." << endl;
        system("pause");
        return;
    }
}

void menu_facturacion(string nombre_archivo_facturas, string nombre_archivo_ventas, vector<string> metodos_pago)
{
    int opcion;

    do
    {
        system("cls");
        cout << "====== MENÚ DE FACTURACION ======" << endl;
        cout << "1. Crear nueva factura" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccionar opcion: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            crear_factura(nombre_archivo_facturas, nombre_archivo_ventas, metodos_pago);
            break;

        default:
            if (opcion != 0)
            {
                cout << "Opción inválida" << endl;
                system("pause");
            }
        }

    } while (opcion != 0);
}
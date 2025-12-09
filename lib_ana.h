#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>

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


void adicionar_venta(string nombre_archivo_ventas)
{
    ventaProducto venta;
    ofstream archivo_ventas;

    archivo_ventas.open(nombre_archivo_ventas, ios::binary | ios::app);
    if (!archivo_ventas.good())
    {
        cout << "No se pudo abrir el archivo de ventas." << endl;
        system("pause");
        return;
    }

    cout << "Ingresar código del producto: ";
    cin >> venta.codigoProducto;

    cin.ignore();
    cout << "Ingresar CI del cliente: ";
    cin.getline(venta.CI_Cliente, 10);

    cout << "Ingresar CI del empleado: ";
    cin.getline(venta.CI_Empleado, 10);

    cout << "Ingresar fecha de venta" << endl;
    cout << "\tDía: ";
    cin >> venta.fechaDeVenta.dia;
    cout << "\tMes: ";
    cin >> venta.fechaDeVenta.mes;
    cout << "\tAño: ";
    cin >> venta.fechaDeVenta.anio;

    cout << "Ingresar cantidad vendida: ";
    cin >> venta.cantidad;

    cout << "Ingresar precio unitario: ";
    cin >> venta.precioUnitario;

    venta.eliminada = false;
    venta.facturada = false;
    // Venta nueva sin facturar.
    venta.numeroFactura = 0;
    // La venta aún no pertenece a ninguna factura.

    archivo_ventas.write((char*)&venta, sizeof(ventaProducto));
    archivo_ventas.close();

    cout << "Venta registrada correctamente." << endl;
    system("pause");
}

void modificar_venta(string nombre_archivo_ventas)
{
    ventaProducto venta;
    fstream archivo_ventas;
    char CI_a_buscar[10];
    int codigo_a_buscar;

    cout << "Ingresar CI del cliente: ";
    cin.ignore();
    cin.getline(CI_a_buscar, 10);

    cout << "Ingresar código del producto a modificar: ";
    cin >> codigo_a_buscar;

    archivo_ventas.open(nombre_archivo_ventas, ios::binary | ios::in | ios::out);
    if (!archivo_ventas.good())
    {
        cout << "No se pudo abrir archivo de ventas." << endl;
        system("pause");
        return;
    }

    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto)))
    {
        if (strcmp(venta.CI_Cliente, CI_a_buscar) == 0 && venta.codigoProducto == codigo_a_buscar && !venta.eliminada && !venta.facturada)
        {
            cout << "Venta encontrada" << endl;

            archivo_ventas.seekp(-sizeof(ventaProducto), ios::cur);

            cout << "Nueva cantidad: ";
            cin >> venta.cantidad;

            cout << "Nuevo precio unitario: ";
            cin >> venta.precioUnitario;

            archivo_ventas.write((char*)&venta, sizeof(ventaProducto));

            cout << "Venta modificada correctamente." << endl;
            archivo_ventas.close();
            system("pause");
            return;
        }
    }

    cout << "La venta no fue encontrada o ya fue facturada." << endl;
    archivo_ventas.close();
    system("pause");
}

void eliminar_venta(string nombre_archivo_ventas)
{
    ventaProducto venta;
    fstream archivo_ventas;
    int codigo_a_buscar;
    char CI_a_buscar[10];

    cout << "Ingresar CI del cliente: ";
    cin.ignore();
    cin.getline(CI_a_buscar, 10);

    cout << "Ingresar código del producto a eliminar: ";
    cin >> codigo_a_buscar;

    archivo_ventas.open(nombre_archivo_ventas, ios::binary | ios::in | ios::out);
    if (!archivo_ventas.good())
    {
        cout << "No se pudo abrir archivo de ventas." << endl;
        system("pause");
        return;
    }

    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto)))
    {
        if (strcmp(venta.CI_Cliente, CI_a_buscar) == 0 && venta.codigoProducto == codigo_a_buscar && !venta.eliminada && !venta.facturada)
        {
            cout << "Venta encontrada" << endl;

            archivo_ventas.seekp(-sizeof(ventaProducto), ios::cur);

            venta.eliminada = true;
            archivo_ventas.write((char*)&venta, sizeof(ventaProducto));

            archivo_ventas.close();

            cout << "Venta eliminada correctamente." << endl;

            system("pause");
            return;
        }
    }

    cout << "La venta no fue encontrada o ya fue facturada." << endl;;
    archivo_ventas.close();
    system("pause");
}


int generar_numero_factura(string nombre_archivo_facturas)
{
    datosFactura factura_actual;
    ifstream archivo_facturas;
    int ultimo_numero_factura = 0;

    archivo_facturas.open(nombre_archivo_facturas, ios::binary);

    if (archivo_facturas.good())
    {
        while (archivo_facturas.read((char*)&factura_actual, sizeof(datosFactura)))
        {
            if (factura_actual.numeroFactura > ultimo_numero_factura)
            {
                ultimo_numero_factura = factura_actual.numeroFactura;
            }
        }
    }
    else
    {
        return 1;
    }

    archivo_facturas.close();
    return ultimo_numero_factura + 1;
}

void crear_factura(string nombre_archivo_facturas, string nombre_archivo_ventas, vector<string> metodos_pago)
{
    ofstream archivo_facturas;
    fstream archivo_ventas;
    datosFactura factura_nueva;
    ventaProducto venta;

    archivo_facturas.open(nombre_archivo_facturas, ios::binary | ios::app);

    if (!archivo_facturas.good())
    {
        cout << "No se pudo abrir archivo de facturas." << endl;
        system("pause");
        return;
    }

    cin.ignore();
    cout << "Ingresar CI Cliente: ";
    cin.getline(factura_nueva.CI_cliente, 10);

    cout << "Ingresar fecha de emisión" << endl;
    cout << "\tDía: "; cin >> factura_nueva.fecha_emision_factura.dia;
    cout << "\tMes: "; cin >> factura_nueva.fecha_emision_factura.mes;
    cout << "\tAño: "; cin >> factura_nueva.fecha_emision_factura.anio;

    cout << "\tMÉTODOS DE PAGO" << endl;
    for (int i = 0; i < metodos_pago.size(); i++)
        cout << "\t" << i + 1 << " " << metodos_pago[i] << endl;

    cout << "Ingresar método de pago: ";
    cin >> factura_nueva.metodo_pago;

    factura_nueva.anulada = false;

    factura_nueva.numeroFactura = generar_numero_factura(nombre_archivo_facturas);
    // Se genera número de factura consecutivo.

    archivo_facturas.write((char*)&factura_nueva, sizeof(datosFactura));
    archivo_facturas.close();


    archivo_ventas.open(nombre_archivo_ventas, ios::binary | ios::in | ios::out);
    // Ahora se actualizan las ventas del cliente para asociarlas a esta factura.
    if (!archivo_ventas.good())
    {
        cout << "No se pudo abrir archivo de ventas." << endl;
        system("pause");
        return;
    }

    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto)))
    {
        if (strcmp(venta.CI_Cliente, factura_nueva.CI_cliente) == 0 && !venta.eliminada && !venta.facturada)
        {
            archivo_ventas.seekp(-sizeof(ventaProducto), ios::cur);

            venta.facturada = true;
            venta.numeroFactura = factura_nueva.numeroFactura;

            archivo_ventas.write((char*)&venta, sizeof(ventaProducto));
        }
    }

    archivo_ventas.close();

    cout << "Factura creada con número: " << factura_nueva.numeroFactura << endl;
    system("pause");
}

void anular_factura(string nombre_archivo_facturas)
{
    datosFactura factura;
    fstream archivo_facturas;
    int numero_a_anular;

    cout << "Ingresar número de factura a anular: ";
    cin >> numero_a_anular;

    archivo_facturas.open(nombre_archivo_facturas, ios::binary | ios::in | ios::out);
    {
        cout << "No se pudo abrir el archivo de facturas." << endl;
        system("pause");
        return;
    }

    while (archivo_facturas.read((char*)&factura, sizeof(datosFactura)))
    {
        if (factura.numeroFactura == numero_a_anular && !factura.anulada)
        {
            archivo_facturas.seekp(-sizeof(datosFactura), ios::cur);
            factura.anulada = true;
            archivo_facturas.write((char*)&factura, sizeof(datosFactura));

            cout << "Factura anulada correctamente." << endl;
            archivo_facturas.close();
            system("pause");
            return;
        }
    }

    cout << "Factura no encontrada o ya anulada." << endl;
    archivo_facturas.close();
    system("pause");
}


void listar_ventas_pendientes(string nombre_archivo_ventas)
{
    ventaProducto venta;
    ifstream archivo_ventas;

    archivo_ventas.open(nombre_archivo_ventas, ios::binary);

    cout << "=== Ventas pendientes de facturar ===" << endl;
    cout << "CI Cliente \tCódigo Producto \tCantidad \tPrecio Unitario " << endl;
    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto)))
    {
        if (!venta.eliminada && !venta.facturada)
        {
            cout << venta.CI_Cliente << "\t" << venta.codigoProducto << "\t" << venta.cantidad << "\t" << venta.precioUnitario << endl;
        }
    }
    archivo_ventas.close();
    system("pause");
}

void menu_facturacion(string nombre_archivo_ventas, string nombre_archivo_facturas, vector<string> metodos_pago)
{
    int opcion;

    do
    {
        system("cls");
        cout << "=== MENÚ DE OPCIONES ===" << endl;
        cout << "1 Adicionar venta" << endl;
        cout << "2 Modificar venta" << endl;
        cout << "3 Eliminar venta" << endl;
        cout << "4 Listar ventas pendientes" << endl;
        cout << "5 Crear factura" << endl;
        cout << "6 Anular factura" << endl;
        cout << "7 Listado " << endl;
        cout << "0 Salir" << endl;
        cout << endl;
        cout << "Seleccionar opción: ";
        cin >> opcion;

        switch(opcion)
        {
            case 1:
                adicionar_venta(nombre_archivo_ventas);
                break;

            case 2:
                modificar_venta(nombre_archivo_ventas);
                break;

            case 3:
                eliminar_venta(nombre_archivo_ventas);
                break;

            case 4:
                listar_ventas_pendientes(nombre_archivo_ventas);
                break;
            
            case 5:
                crear_factura(nombre_archivo_facturas, nombre_archivo_ventas, metodos_pago);
                break;
            
            case 6:
                anular_factura(nombre_archivo_facturas);
                break;
            
            case 7:
                /* code */
                break;

            default:
            if (opcion != 0)
            {
                cout << "Opción inválida." << endl;
                system("pause");
            }
            break;
        }

    } while(opcion != 0);
}

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
    char CI_Cliente[10];
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
    char CI_Cliente[10];
    char CI_Empleado[10];
    Fecha fechaDeVenta;
    int codigoProducto;
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

void crear_productos_prueba(string nombre_archivo_productos)
{
    ofstream archivo_productos;
    
    archivo_productos.open(nombre_archivo_productos, ios::binary);

    if (!archivo_productos.good())
    {
        cout << "No se pudo crear productos de prueba." << endl;
        return;
    }

    DatosProducto p1 = {1, "Galaxy S23", 1, 4500.00, 4, false};
    DatosProducto p2 = {2, "Galaxy A54", 1, 2800.00, 10, false};
    DatosProducto p3 = {3, "Galaxy Tab", 2, 3200.00, 2, false};

    archivo_productos.write((char*)&p1, sizeof(DatosProducto));
    archivo_productos.write((char*)&p2, sizeof(DatosProducto));
    archivo_productos.write((char*)&p3, sizeof(DatosProducto));

    archivo_productos.close();
}












// ============================== FUNCIONES REPORTES ==============================

void reporte_ventas_cliente(string nombre_archivo_ventas)
{
    ventaProducto venta;
    ifstream archivo_ventas;
    char CI_a_buscar[10];
    float subtotal = 0.0;
    float total = 0.0;

    cin.ignore();
    cout << "Ingresar CI del cliente: ";
    cin.getline(CI_a_buscar, 10);

    archivo_ventas.open(nombre_archivo_ventas, ios::binary);
    if (!archivo_ventas.good())
    {
        cout << "No se pudo abrir el archivo de ventas." << endl;
        return;
    }

    cout << "CÓDIGO \tFECHA \tCANTIDAD \tPRECIO \tSUBTOTAL" << endl;

    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto)))
    {
        if (strcmp(venta.CI_Cliente, CI_a_buscar) == 0 && venta.facturada && !venta.eliminada)
        {
            subtotal = venta.cantidad * venta.precioUnitario;
            total = total + subtotal;

            cout << venta.codigoProducto << "\t" << venta.fechaDeVenta.dia << "/" << venta.fechaDeVenta.mes << "/" << venta.fechaDeVenta.anio << "\t" << venta.cantidad << "\t" << venta.precioUnitario << "\t" << subtotal << endl;
        }
    }

    archivo_ventas.close();

    cout << "TOTAL COMPRADO: " << total << endl;
    system("pause");
}

void reporte_ventas_diarias(string nombre_archivo_ventas)
{
    ventaProducto venta;
    ifstream archivo_ventas;
    Fecha fecha;
    float subtotal = 0.0;
    float total = 0.0;

    cout << "Ingresar fecha" << endl;
    cout << "Día: ";
    cin >> fecha.dia;
    cout << "Mes: ";
    cin >> fecha.mes;
    cout << "Año: ";
    cin >> fecha.anio;

    archivo_ventas.open(nombre_archivo_ventas, ios::binary);

    cout << "CÓDIGO \tCI CLIENTE \tCANTIDAD \tSUBTOTAL" << endl;

    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto)))
    {
        if (venta.facturada && venta.fechaDeVenta.dia == fecha.dia && venta.fechaDeVenta.mes == fecha.mes && venta.fechaDeVenta.anio == fecha.anio)
        {
            subtotal = venta.cantidad * venta.precioUnitario;
            total = total + subtotal;

            cout << venta.codigoProducto << "\t" << venta.CI_Cliente << "\t" << venta.cantidad << "\t" << subtotal << endl;
        }
    }

    archivo_ventas.close();
    cout << "TOTAL DEL DÍA: " << total << endl;
    system("pause");
}

void reporte_diario_productos_disponibles(string nombre_archivo_productos)
{
    DatosProducto producto;
    ifstream archivo_productos;

    archivo_productos.open(nombre_archivo_productos, ios::binary);

    cout << "CÓDIGO \tMODELO \tSTOCK" << endl;

    while (archivo_productos.read((char*)&producto, sizeof(DatosProducto)))
    {
        if (!producto.eliminado && producto.stock > 0)
        {
            cout << producto.codigo << "\t" << producto.modelo << "\t" << producto.stock << endl;
        }
    }

    archivo_productos.close();
    system("pause");
}

void reporte_temporada_alta(string nombre_archivo_ventas)
{
    ventaProducto venta;
    ifstream archivo_ventas;
    vector<int> ventas_por_mes(13, 0);
    // Vector con 13 posiciones: 1–12 son los meses.
    vector<string> nombres_meses = {"", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    int mes_con_mayor_venta = 0;
    int cantidad_maxima_vendida = 0;

    archivo_ventas.open(nombre_archivo_ventas, ios::binary);
    if (!archivo_ventas.good())
    {
        cout << "No se pudo abrir el archivo de ventas." << endl;
        return;
    }

    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto)))
    {
        if (venta.facturada && !venta.eliminada)
        {
            ventas_por_mes[venta.fechaDeVenta.mes] = ventas_por_mes[venta.fechaDeVenta.mes] + venta.cantidad;
        }
    }
    archivo_ventas.close();

    for (int i = 1; i <= 12; i++)
    {
        if (ventas_por_mes[i] > cantidad_maxima_vendida)
        {
            cantidad_maxima_vendida = ventas_por_mes[i];
            mes_con_mayor_venta = i;
        }
    }

    if (mes_con_mayor_venta != 0)
    {
        cout << "Mes con más ventas: " << nombres_meses[mes_con_mayor_venta] << " con " << cantidad_maxima_vendida << " productos vendidos." << endl;
    }
    else
    {
        cout << "No hay ventas registradas." << endl;
    }

    system("pause");
}



// ============================== FUNCIONES VENTA ==============================


bool buscar_producto(int codigo, DatosProducto &producto_encontrado, string nombre_archivo)
{
    ifstream archivo_productos;
    
    archivo_productos.open(nombre_archivo, ios::binary);

    if (!archivo_productos.good())
    {
        cout << "No se pudo abrir el archivo de productos." << endl;
        return false;
    }
    while (archivo_productos.read((char*)&producto_encontrado, sizeof(DatosProducto)))
    {
        if (producto_encontrado.codigo == codigo && !producto_encontrado.eliminado)
        {
            return true;
        }
    }

    archivo_productos.close();
    return false;
}

void adicionar_venta(string nombre_archivo_ventas, string nombre_archivo_productos)
{
    ventaProducto venta;
    DatosEmpleado empleado;
    DatosProducto producto;
    ofstream archivo_ventas;

    archivo_ventas.open(nombre_archivo_ventas, ios::binary | ios::app);
    if (!archivo_ventas.good())
    {
        cout << "No se pudo abrir el archivo de ventas." << endl;
        system("pause");
        return;
    }
   
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
    
    cout << "Ingresar código del producto: ";
    cin >> venta.codigoProducto;

    if (!buscar_producto(venta.codigoProducto, producto, nombre_archivo_productos))
    {
        cout << "Producto no encontrado." << endl;
        system("pause");
        return;
    }

    cout << "Producto: " << producto.modelo << endl;
    cout << "Precio: " << producto.precioVenta << endl;
    cout << "Stock disponible: " << producto.stock << endl;
    venta.precioUnitario = producto.precioVenta;
    cout << "Precio unitario: " << venta.precioUnitario;
    cout << endl;
    cout << "Ingresar cantidad a vender: ";
    cin >> venta.cantidad;

    if (venta.cantidad > producto.stock)
    {
        cout << "Stock insuficiente." << endl;
        system("pause");
        return;
    }
    
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
    bool venta_modificada = false;

    cout << "Ingresar CI del cliente: ";
    cin.ignore();
    cin.getline(CI_a_buscar, 10);

    cout << "Ingresar código del producto a modificar: ";
    cin >> codigo_a_buscar;

    archivo_ventas.open(nombre_archivo_ventas, ios::binary | ios::in | ios::out);
    if (!archivo_ventas.good())
    {
        cout << "No se pudo abrir el archivo de ventas." << endl;
        system("pause");
        return;
    }

    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto)))
    {
        if (strcmp(venta.CI_Cliente, CI_a_buscar) == 0 && venta.codigoProducto == codigo_a_buscar && !venta.eliminada && !venta.facturada)
        {
            cout << "VENTA ENCONTRADA" << endl;

            archivo_ventas.seekp(-sizeof(ventaProducto), ios::cur);

            cout << "Nueva cantidad: ";
            cin >> venta.cantidad;

            archivo_ventas.write((char*)&venta, sizeof(ventaProducto));

            venta_modificada = true;
            cout << "Venta modificada correctamente." << endl;
        }
    }
    if (!venta_modificada)
    {
        cout << "La venta no fue encontrada o ya fue facturada." << endl;
    }

    archivo_ventas.close();
    system("pause");
}

void eliminar_venta(string nombre_archivo_ventas)
{
    ventaProducto venta;
    fstream archivo_ventas;
    int codigo_a_buscar;
    char CI_a_buscar[10];
    bool venta_encontrada = false;

    cout << "Ingresar CI del cliente: ";
    cin.ignore();
    cin.getline(CI_a_buscar, 10);

    cout << "Ingresar código del producto a eliminar: ";
    cin >> codigo_a_buscar;

    archivo_ventas.open(nombre_archivo_ventas, ios::binary | ios::in | ios::out);
    if (!archivo_ventas.good())
    {
        cout << "No se pudo abrir el archivo de ventas." << endl;
        system("pause");
        return;
    }

    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto)))
    {
        if (strcmp(venta.CI_Cliente, CI_a_buscar) == 0 && venta.codigoProducto == codigo_a_buscar && !venta.eliminada && !venta.facturada)
        {
            cout << "VENTA ENCONTRADA" << endl;

            archivo_ventas.seekp(-sizeof(ventaProducto), ios::cur);

            venta.eliminada = true;
            archivo_ventas.write((char*)&venta, sizeof(ventaProducto));

            venta_encontrada = true;

            cout << "Venta eliminada correctamente." << endl;

        }
    }
    if (!venta_encontrada)
    {
        cout << "La venta no fue encontrada o ya fue facturada." << endl;
    }
    
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

void listar_ventas_pendientes(string nombre_archivo_ventas)
{
    ventaProducto venta;
    ifstream archivo_ventas;

    archivo_ventas.open(nombre_archivo_ventas, ios::binary);

    cout << "======= VENTAS PENDIENTES DE FACTURAR =======" << endl;
    cout << "CI CLIENTE \tCÓDIGO PRODUCTO \tCANTIDAD \tPRECIO UNITARIO " << endl;
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



// ============================== FUNCIONES FACTURACIÓN ==============================

void alerta_stock_producto(int codigoProducto, string nombre_archivo_productos, int limite)
{
    DatosProducto producto;
    ifstream archivo_productos;

    archivo_productos.open(nombre_archivo_productos, ios::binary);

    if (!archivo_productos.good())
    {
        cout << "No se pudo abrir el archivo de productos para verificar stock.";
        return;
    }

    while (archivo_productos.read((char*)&producto, sizeof(DatosProducto)))
    {
        if (producto.codigo == codigoProducto && !producto.eliminado)
        {
            if (producto.stock <= limite)
            {
                cout << endl;
                cout << endl;
                cout << "====================" << endl;
                cout << "ALERTA DE STOCK BAJO" << endl;
                cout << "Producto: " << producto.modelo << endl;
                cout << "Stock actual: " << producto.stock << endl;
                cout << "====================" << endl;
                cout << endl;
            }
        }
    }
    archivo_productos.close();
}

void descontar_stock(int codigoProducto, int cantidad_vendida, string nombre_archivo_productos)
{
    DatosProducto producto;
    fstream archivo_productos;

    archivo_productos.open(nombre_archivo_productos, ios::binary | ios::in | ios::out);
    if (!archivo_productos.good())
    {   
        cout << "No se pudo abrir archivo de productos." << endl;
        return;
    }

    while (archivo_productos.read((char*)&producto, sizeof(DatosProducto)))
    {
        if (producto.codigo == codigoProducto && !producto.eliminado)
        {
            archivo_productos.seekp(-sizeof(DatosProducto), ios::cur);
            producto.stock = producto.stock - cantidad_vendida;
            archivo_productos.write((char*)&producto, sizeof(DatosProducto));
        }
    }
    archivo_productos.close();
}

void sumar_stock(int codigoProducto, int cantidad_devuelta, string nombre_archivo_productos)
{
    DatosProducto producto;
    fstream archivo_productos;

    archivo_productos.open(nombre_archivo_productos, ios::binary | ios::in | ios::out);
    if (!archivo_productos.good())
    {
        cout << "No se pudo abrir el archivo de productos." << endl;
        return;
    }

    while (archivo_productos.read((char*)&producto, sizeof(DatosProducto)))
    {
        if (producto.codigo == codigoProducto && !producto.eliminado)
        {
            archivo_productos.seekp(-sizeof(DatosProducto), ios::cur);
            producto.stock = producto.stock + cantidad_devuelta;
            archivo_productos.write((char*)&producto, sizeof(DatosProducto));
        }
    }
    archivo_productos.close();
}

void crear_factura(string nombre_archivo_facturas, string nombre_archivo_ventas, vector<string> metodos_pago, string nombre_archivo_productos)
{
    ofstream archivo_facturas;
    fstream archivo_ventas;
    datosFactura factura_nueva;
    ventaProducto venta;

    bool fecha_asignada = false;

    archivo_facturas.open(nombre_archivo_facturas, ios::binary | ios::app);

    if (!archivo_facturas.good())
    {
        cout << "No se pudo abrir el archivo de facturas." << endl;
        system("pause");
        return;
    }

    cin.ignore();
    cout << "Ingresar CI Cliente: ";
    cin.getline(factura_nueva.CI_cliente, 10);

    archivo_ventas.open(nombre_archivo_ventas, ios::binary | ios::in);
    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto)))
    {
        if (strcmp(venta.CI_Cliente, factura_nueva.CI_cliente) == 0 && !venta.eliminada && !venta.facturada)
        {
            if (!fecha_asignada)
            {
                factura_nueva.fecha_emision_factura = venta.fechaDeVenta;
                fecha_asignada = true;
            }
        }
    }
    archivo_ventas.close();

    if (!fecha_asignada)
    {
        cout << "El cliente no tiene ventas pendientes para facturar." << endl;
        system("pause");
        return;
    }
    
    cout << "Fecha de emisión: " << factura_nueva.fecha_emision_factura.dia << "/" << factura_nueva.fecha_emision_factura.mes << "/" << factura_nueva.fecha_emision_factura.anio << endl;
    // Se obtiene la fecha de emisión de la factura a partir de la primera venta pendiente del cliente.

    cout << "\tMÉTODOS DE PAGO" << endl;
    for (int i = 0; i < metodos_pago.size(); i++)
        cout << "\t" << i + 1 << " " << metodos_pago[i] << endl;

    cout << "Ingresar método de pago: ";
    cin >> factura_nueva.metodo_pago;

    factura_nueva.anulada = false;

    factura_nueva.numeroFactura = generar_numero_factura(nombre_archivo_facturas);
    // Se genera número de factura.

    archivo_facturas.write((char*)&factura_nueva, sizeof(datosFactura));
    archivo_facturas.close();


    archivo_ventas.open(nombre_archivo_ventas, ios::binary | ios::in | ios::out);
    // Se recorren nuevamente las ventas para marcarlas como facturadas y asociarlas al número de factura generado.
    if (!archivo_ventas.good())
    {
        cout << "No se pudo abrir el archivo de ventas." << endl;
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

            descontar_stock(venta.codigoProducto, venta.cantidad, nombre_archivo_productos);
            // CÓMO SE LLAMA EL ARCHIVO DE PRODUCTOS????
            alerta_stock_producto(venta.codigoProducto, nombre_archivo_productos, 5);












            archivo_ventas.write((char*)&venta, sizeof(ventaProducto));
        }
    }

    archivo_ventas.close();

    cout << "Factura creada con número: " << factura_nueva.numeroFactura << endl;

    system("pause");
}

void anular_factura(string nombre_archivo_facturas, string nombre_archivo_ventas, string nombre_archivo_productos)
{
    datosFactura factura;
    ventaProducto venta;
    fstream archivo_facturas;
    fstream archivo_ventas;
    int numero_factura_a_anular;
    bool bool_factura_anulada = false;

    cout << "Ingresar número de factura a anular: ";
    cin >> numero_factura_a_anular;

    archivo_facturas.open(nombre_archivo_facturas, ios::binary | ios::in | ios::out);
    if (!archivo_facturas.good())
    {
        cout << "No se pudo abrir el archivo de facturas." << endl;
        system("pause");
        return;
    }

    while (archivo_facturas.read((char*)&factura, sizeof(datosFactura)))
    {
        if (factura.numeroFactura == numero_factura_a_anular && !factura.anulada && !bool_factura_anulada)
        {
            archivo_facturas.seekp(-sizeof(datosFactura), ios::cur);
            factura.anulada = true;
            archivo_facturas.write((char*)&factura, sizeof(datosFactura));
            bool_factura_anulada = true;
        }
    }
    archivo_facturas.close();

    if (!bool_factura_anulada)
    {
        cout << "Factura no encontrada o ya anulada." << endl;
        system("pause");
        return;
    }
    
    archivo_ventas.open(nombre_archivo_ventas, ios::binary | ios::in | ios::out);
    if (!archivo_ventas.good())
    {
        cout << "No se pudo abrir el archivo de ventas." << endl;
        system("pause");
        return;
    }
    
    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto)))
    {
        if (venta.numeroFactura == numero_factura_a_anular)
        {
            archivo_ventas.seekp(-sizeof(ventaProducto), ios::cur);

            venta.facturada = false;
            // La venta vuelve a estar pendiente.
            venta.numeroFactura = 0;
            // No pertenece a ninguna factura.
            venta.eliminada = true;
            // La venta ahora ya fue eliminada.

            // Al anular la factura, las ventas asociadas se marcan como eliminadas para evitar que vuelvan a aparecer en reportes.

            archivo_ventas.write((char*)&venta, sizeof(ventaProducto));

            sumar_stock(venta.codigoProducto, venta.cantidad, nombre_archivo_productos);
            // Se devuelve el stock.

            // CÓMO SE LLAMA EL ARCHIVO DE PRODUCTOS?











        }
    }
    archivo_ventas.close();

    cout << "Factura anulada correctamente y stock actualizado." << endl;

    system("pause");
}

void mostrar_detalle_factura(string nombre_archivo_facturas, string nombre_archivo_ventas, string nombre_archivo_productos)
{
    int numero_factura_a_buscar;
    datosFactura factura;
    ventaProducto venta;
    DatosProducto producto;
    ifstream archivo_facturas;
    ifstream archivo_ventas;
    ifstream archivo_productos;
    bool factura_encontrada = false;
    float subtotal = 0.0;
    float total = 0.0;
    string estado_factura;

    cout << "Ingresar número de factura a buscar: ";
    cin >> numero_factura_a_buscar;

    archivo_facturas.open(nombre_archivo_facturas, ios::binary);
    if (!archivo_facturas.good())
    {
        cout << "No se pudo abrir archivo de facturas para ver el detalle." << endl;
        return;
    }

    while (archivo_facturas.read((char*)&factura, sizeof(datosFactura)))
    {
        if (factura.numeroFactura == numero_factura_a_buscar && !factura_encontrada)
        {
            factura_encontrada = true;
        }
    }
    archivo_facturas.close();

    if (!factura_encontrada)
    {
        cout << "Factura no encontrada." << endl;
        return;
    }

    if (!factura.anulada)
        {
            estado_factura = "FACTURA ACTIVA";
        }
        else
        {
            estado_factura = "FACTURA ANULADA";
        }

    cout << "======= DETALLE FACTURA =======" << endl;

    cout << "Estado: " << estado_factura << endl;
    cout << endl;
    cout << "Número: " << factura.numeroFactura << endl;
    cout << "CI Cliente: " << factura.CI_cliente << endl;
    cout << "Fecha: " << factura.fecha_emision_factura.dia << "/" << factura.fecha_emision_factura.mes << "/" << factura.fecha_emision_factura.anio << endl;
    cout << endl;
    cout << "CÓDIGO PRODUCTO \tDESCRIPCIÓN \tCANTIDAD \tPRECIO UNITARIO \tSUBTOTAL" << endl;

    archivo_ventas.open(nombre_archivo_ventas, ios::binary);
    if (!archivo_ventas.good())
    {
        cout << "No se pudo abrir archivo de ventas." << endl;
        return;
    }

    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto)))
    {
        if (venta.numeroFactura == numero_factura_a_buscar && !venta.eliminada)
        {
            subtotal = venta.cantidad * venta.precioUnitario;
            total = total + subtotal;

            archivo_productos.open(nombre_archivo_productos, ios::binary);
            if (!archivo_productos.good())
            {
                cout << "No se pudo abrir el archivo de productos" << endl;
                return;
            }
            
            cout << venta.codigoProducto << "\t" << producto.categoria << "-" << producto.modelo << "\t" << venta.cantidad << "\t" << venta.precioUnitario << "\t" << subtotal << endl;
            
            archivo_productos.close();
        }
    }
    archivo_ventas.close();
    
    cout << endl;
    cout << "TOTAL: " << total << endl;
    cout << "==========================================" << endl;

    system("pause");
}

void listar_facturas(string nombre_archivo_facturas, vector<string> metodos_pago, string nombre_archivo_ventas)
{
    datosFactura factura;
    ifstream archivo_facturas;
    string estado_factura;

    archivo_facturas.open(nombre_archivo_facturas, ios::binary);
    if (!archivo_facturas.good())
    {
        cout << "No se pudo abrir el archivo para ver el listado de facturas." << endl;
        system("pause");
        return;
    }

    cout << "N° FACTURA \tCI CLIENTE \tFECHA DE EMISIÓN \tMÉTODO DE PAGO \tESTADO" << endl;

    while (archivo_facturas.read((char*)&factura, sizeof(datosFactura)))
    {
        if (!factura.anulada)
        {
            estado_factura = "ACTIVA";
        }
        else
        {
            estado_factura = "ANULADA";
        }

        cout << factura.numeroFactura << "\t" << factura.CI_cliente << "\t" << factura.fecha_emision_factura.dia << "/" << factura.fecha_emision_factura.mes << "/" << factura.fecha_emision_factura.anio << "\t" << metodos_pago[factura.metodo_pago - 1] << "\t" << estado_factura << endl;
    }

    archivo_facturas.close();
    system("pause");
}




// ============================== FUNCIONES MENÚS ==============================

void menu_reportes(string nombre_archivo_ventas, string nombre_archivo_productos)
{
    int opcion;

    do
    {
        system("cls");
        cout << "======= MENÚ DE REPORTES =======" << endl;
        cout << "1. Ventas por cliente" << endl;
        cout << "2. Ventas diarias" << endl;
        cout << "3. Productos disponibles" << endl;
        cout << "4. Temporada alta de ventas por mes ??????????????????????????????????????????????????????????????????????????????" << endl;
        cout << "0. Volver" << endl;
        cout << endl;
        cout << "Seleccionar opción: ";
        cin >> opcion;

        switch (opcion)
        {
            case 1:
                reporte_ventas_cliente(nombre_archivo_ventas);
                break;

            case 2:
                reporte_ventas_diarias(nombre_archivo_ventas);
                break;

            case 3:
                reporte_diario_productos_disponibles(nombre_archivo_productos);
                break;

            case 4:
                reporte_temporada_alta(nombre_archivo_ventas);
                break;

            default:
                if (opcion != 0)
                {
                    cout << "Opción inválida." << endl;
                    system("pause");
                }
                break;
        }
    } while (opcion != 0);
}

void menu_facturacion(string nombre_archivo_ventas, string nombre_archivo_facturas, vector<string> metodos_pago, string nombre_archivo_productos)
{
    int opcion;

    do
    {
        system("cls");
        cout << "======= MENÚ DE OPCIONES =======" << endl;
        cout << "1 Adicionar venta" << endl;
        cout << "2 Modificar venta" << endl;
        cout << "3 Eliminar venta" << endl;
        cout << "4 Listado de ventas pendientes" << endl;
        cout << "5 Crear factura" << endl;
        cout << "6 Anular factura" << endl;
        cout << "7 Listado de facturas emitidas" << endl;
        cout << "0 Volver a Menú Principal" << endl;
        cout << endl;
        cout << "Seleccionar opción: ";
        cin >> opcion;

        switch(opcion)
        {
            case 1:
                adicionar_venta(nombre_archivo_ventas, nombre_archivo_productos);
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
                crear_factura(nombre_archivo_facturas, nombre_archivo_ventas, metodos_pago, nombre_archivo_productos);
                break;
            
            case 6:
                anular_factura(nombre_archivo_facturas, nombre_archivo_ventas, nombre_archivo_productos);
                break;
            
            case 7:
                listar_facturas(nombre_archivo_facturas, metodos_pago, nombre_archivo_ventas);
                break;

            case 8:
                menu_mostrar_detalle_factura(nombre_archivo_ventas, nombre_archivo_facturas);
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

void menu_principal(string nombre_archivo_ventas, string nombre_archivo_facturas, string nombre_archivo_productos, vector<string> metodos_pago)
{
    int opcion;

    do
    {
        system("cls");
        cout << "======= SISTEMA VENTAS TIENDA SAMSUNG =======" << endl;
        cout << "1 Gestión de ventas y facturación" << endl;
        cout << "2 Reportes" << endl;
        cout << "0 Salir" << endl;
        cout << endl;
        cout << "Seleccionar opción: ";
        cin >> opcion;

        switch (opcion)
        {
            case 1:
                menu_facturacion(nombre_archivo_ventas, nombre_archivo_facturas, metodos_pago, nombre_archivo_productos);
                break;

            case 2:
                menu_reportes(nombre_archivo_ventas, nombre_archivo_productos);
                break;

            default:
                if (opcion != 0)
                {
                    cout << "Opción inválida." << endl;
                    system("pause");
                }
                break;
        }

    } while (opcion != 0);
}

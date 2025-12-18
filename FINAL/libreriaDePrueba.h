#include <iostream>

#include <fstream>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

vector<string> meses = {"","Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
vector<string> categoriasProductoVector = {"Celular", "Tablet", "Laptop", "Televisor", "Monitor", "Parlante", "Proyector", "Lavadora", "Refrigerador"};
vector<int> diasMeses = {0,31,28,31,30,31,30,31,31,30,31,30,31};
vector<int> diasMesesBisiesto = {0,31,28,31,30,31,30,31,31,30,31,30,31}; // se deja el primer componente como 0 para que vaya de acuerdo a los meses

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
    bool eliminado;
};

struct DatosEmpleado
{
    char CI_Empleado[10];
    char nombre[30];
    char apellido[30];
    int rango;
    bool eliminado;
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


// ------------------------------------- CLIENTES ------------------------------------------------------------------------------


void llenarDatosCliente(string nombreArchivo) {
    DatosCliente c;
    system("cls");
    cout << endl << "=== INGRESO DE NUEVO CLIENTE ===" << endl;
    cout << "Ingrese el CI del cliente: ";
    cin.getline(c.CI_Cliente, 10);
    cout << "Ingrese el nombre del cliente: ";
    cin.getline(c.nombre, 30);
    cout << "Ingrese el apellido del cliente: ";
    cin.getline(c.apellido, 30);
    cout << "Ingrese la fecha de nacimiento del cliente" << endl;
    cout << "Ingrese el Día: ";
    cin >> c.fechaNacimiento.dia;
    cout << "Ingrese el Mes: ";
    cin >> c.fechaNacimiento.mes;
    cout << "Ingrese el Año: ";
    cin >> c.fechaNacimiento.anio;
    cout << "Ingrese el teléfono del cliente: ";
    cin >> c.telefono;
    cin.ignore();
    cout << "Ingrese el correo del cliente: ";
    cin.getline(c.correo, 30);
    
    c.eliminado = false;
    
    ofstream archivo;
    archivo.open(nombreArchivo, ios::binary | ios::app);
    
    if (!archivo.is_open()) {
        cout << "Error: no se pudo guardar clientes.bin" << endl;
        return;
    }
    
    archivo.write((char*)&c, sizeof(DatosCliente));
    archivo.close();
    cout << "Cliente guardado exitosamente en clientes.bin" << endl;
    system("pause");
}

void mostrarTodosClientes(string nombreArchivo) {
    ifstream archivo;
    DatosCliente c;
    int contador = 0;
    int activos = 0;
    system("cls");
    
    archivo.open(nombreArchivo, ios::binary);
    
    if (!archivo.is_open()) {
        cout << "No hay clientes registrados o error al abrir el archivo." << endl;
        return;
    }
    
    cout << endl << "=== LISTA DE TODOS LOS CLIENTES ===" << endl;
    
    while (archivo.read((char*)&c, sizeof(DatosCliente))) {
        contador++;
        if (!c.eliminado) {
            activos++;
            cout << endl << "CLIENTE " << activos << ":" << endl;
            cout << "CI: " << c.CI_Cliente << endl;
            cout << "Nombre: " << c.nombre << " " << c.apellido << endl;
            cout << "Fecha Nacimiento: " << c.fechaNacimiento.mes << "/" << c.fechaNacimiento.dia << "/" << c.fechaNacimiento.anio << endl;
            cout << "Telefono: " << c.telefono << endl;
            cout << "Correo: " << c.correo << endl;
            cout << "--------------------------------------" << endl;
        }
    }
    
    archivo.close();

    
    if (contador == 0) {
        cout << "No hay clientes registrados en el archivo." << endl;
    } else {
        cout << endl << "Total de clientes registrados: " << contador << endl;
        cout << "Clientes activos: " << activos << endl;
        cout << "Clientes eliminados (no mostrados): " << (contador - activos) << endl;
    }
    system("pause");
}

void eliminarCliente(string nombreArchivo) {
    char ciEliminar[10];
    fstream archivo;
    DatosCliente c;
    bool encontrado = false;
    
    cout << endl << "=== ELIMINAR CLIENTE ===" << endl;
    cout << "Ingrese el CI del cliente a eliminar: ";
    cin.getline(ciEliminar, 10);
    
    archivo.open(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << "No hay clientes registrados para eliminar" << endl;
        return;
    }
    
    while (archivo.read((char*)&c, sizeof(DatosCliente)) && !encontrado) {
        if (strcmp(c.CI_Cliente, ciEliminar) == 0 && !c.eliminado) {
            encontrado = true;
            cout << "Cliente encontrado: " << c.nombre << " " << c.apellido << endl;
            cout << "Esta seguro de marcar como eliminado? (s/n): ";
            char confirmacion;
            cin >> confirmacion;
            cin.ignore();
            
            if (confirmacion == 's' || confirmacion == 'S') {
                c.eliminado = true;
                archivo.seekp(-sizeof(DatosCliente), ios::cur);
                archivo.write((char*)&c, sizeof(DatosCliente));
                archivo.flush();
                cout << "Cliente marcado como ELIMINADO correctamente." << endl;
                system("pause");
            } else {
                cout << "Operación cancelada." << endl;
                system("pause");
            }
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << "No se encontró cliente activo con CI: " << ciEliminar << endl;
        system("pause");
    }
}

void buscarClientePorCI(string nombreArchivo) {
    char ciBuscar[10];
    ifstream archivo;
    DatosCliente c;
    bool encontrado = false;
    
    cout << endl << "=== BUSCAR CLIENTE POR CI ===" << endl;
    cout << "Ingrese el CI del cliente a buscar: ";
    cin.getline(ciBuscar, 10);
    
    archivo.open(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << "No hay clientes registrados." << endl;
        return;
    }
    
    while (archivo.read((char*)&c, sizeof(DatosCliente))) {
        if (strcmp(c.CI_Cliente, ciBuscar) == 0 && !c.eliminado) {
            encontrado = true;
            cout << endl << "CLIENTE ENCONTRADO:" << endl;
            cout << "CI: " << c.CI_Cliente << endl;
            cout << "Nombre: " << c.nombre << " " << c.apellido << endl;
            cout << "Fecha Nacimiento: " << c.fechaNacimiento.dia << "/" << c.fechaNacimiento.mes << "/" << c.fechaNacimiento.anio << endl;
            cout << "Telefono: " << c.telefono << endl;
            cout << "Correo: " << c.correo << endl;
            system("pause");
            break;
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << "No se encontró cliente activo con CI: " << ciBuscar << endl;
        system("pause");
    }
}

void modificarCliente(string nombreArchivo) {
    char ciModificar[10];
    fstream archivo;
    DatosCliente c;
    bool encontrado = false;
    int opcion;
    
    cout << endl << "=== MODIFICAR CLIENTE ===" << endl;
    cout << "Ingrese el CI del cliente a modificar: ";
    cin.getline(ciModificar, 10);
    
    archivo.open(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << "No hay clientes registrados para modificar" << endl;
        system("pause");
        return;
    }
    
    while (archivo.read((char*)&c, sizeof(DatosCliente)) && !encontrado) {
        if (strcmp(c.CI_Cliente, ciModificar) == 0 && !c.eliminado) {
            encontrado = true;
            
            cout << endl << "CLIENTE ENCONTRADO:" << endl;
            cout << "CI: " << c.CI_Cliente << endl;
            cout << "Nombre: " << c.nombre << " " << c.apellido << endl;
            cout << "Telefono: " << c.telefono << endl;
            cout << "Correo: " << c.correo << endl;
            
            do {
                cout << endl << "¿Qué dato desea modificar?" << endl;
                cout << "1. Nombre" << endl;
                cout << "2. Apellido" << endl;
                cout << "3. Telefono" << endl;
                cout << "4. Correo electrónico" << endl;
                cout << "5. Fecha de nacimiento" << endl;
                cout << "6. Salir de modificacion" << endl;
                cout << "Seleccione una opción: ";
                cin >> opcion;
                cin.ignore();
                
                switch(opcion) {
                    case 1:
                        cout << "Ingrese el nuevo nombre: ";
                        cin.getline(c.nombre, 30);
                        cout << "Nombre actualizado correctamente." << endl;
                        break;
                    case 2:
                        cout << "Ingrese el nuevo apellido: ";
                        cin.getline(c.apellido, 30);
                        cout << "Apellido actualizado correctamente." << endl;
                        break;
                    case 3:
                        cout << "Ingrese el nuevo telefono: ";
                        cin >> c.telefono;
                        cin.ignore();
                        cout << "Telefono actualizado correctamente." << endl;
                        break;
                    case 4:
                        cout << "Ingrese el nuevo correo electrónico: ";
                        cin.getline(c.correo, 30);
                        cout << "Correo electronico actualizado correctamente." << endl;
                        break;
                    case 5:
                        cout << "Ingrese la nueva fecha de nacimiento:" << endl;
                        cout << "Día: ";
                        cin >> c.fechaNacimiento.dia;
                        cout << "Mes: ";
                        cin >> c.fechaNacimiento.mes;
                        cout << "Anio: ";
                        cin >> c.fechaNacimiento.anio;
                        cin.ignore();
                        cout << "Fecha de nacimiento actualizada correctamente." << endl;
                        break;
                    case 6:
                        cout << "Saliendo de la modificacion..." << endl;
                        break;
                    default:
                        cout << "Opcion no válida. Intente de nuevo." << endl;
                }
                
            } while (opcion != 6);
            
            archivo.seekp(-sizeof(DatosCliente), ios::cur);
            archivo.write((char*)&c, sizeof(DatosCliente));
            archivo.flush();
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << "No se encontró cliente activo con CI: " << ciModificar << endl;
    } else {
        cout << "Cliente modificado correctamente." << endl;
    }
    system("pause");
}


void menuClientes(string nombreArchivo) {
    int opcion;
    do {
        system("cls");
        cout << endl << "=== MENÚ DE GESTIÓN DE CLIENTES ===" << endl;
        cout << "1. Registrar nuevo cliente" << endl;
        cout << "2. Mostrar todos los clientes (solo activos)" << endl;
        cout << "3. Buscar cliente por CI" << endl;
        cout << "4. Modificar cliente por CI" << endl;
        cout << "5. Eliminar cliente por CI (eliminacion logica)" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();
        
        switch(opcion) {
            case 1:
                llenarDatosCliente(nombreArchivo);
                break;
            case 2:
                mostrarTodosClientes(nombreArchivo);
                break;
            case 3:
                buscarClientePorCI(nombreArchivo);
                break;
            case 4:
                modificarCliente(nombreArchivo);
                break;
            case 5:
                eliminarCliente(nombreArchivo);
                break;
            case 0:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcion != 0);
}


// ---------------------------------------------------- EMPLEADO --------------------------------------------------------

void llenarDatosEmpleado(string nombreArchivo) {
    DatosEmpleado e;
    cout << endl << "=== INGRESO DE NUEVO EMPLEADO ===" << endl;
    cout << "Ingrese el CI del empleado: ";
    cin.getline(e.CI_Empleado, 10);
    cout << "Ingrese el nombre del empleado: ";
    cin.getline(e.nombre, 30);
    cout << "Ingrese el apellido del empleado: ";
    cin.getline(e.apellido, 30);
    cout << "Ingrese el rango del empleado:" << endl;
    cout << "1. Vendedor" << endl;
    cout << "2. Cajero" << endl;
    cout << "Seleccione: ";
    cin >> e.rango;
    cin.ignore();
    
    e.eliminado = false;
    
    ofstream archivo;
    archivo.open(nombreArchivo, ios::binary | ios::app);
    
    if (!archivo.is_open()) {
        cout << "Error: no se pudo crear/abrir empleado.bin" << endl;
        return;
    }
    
    archivo.write((char*)&e, sizeof(DatosEmpleado));
    archivo.close();
    cout << "Empleado guardado exitosamente en empleado.bin" << endl;
}


void mostrarTodosEmpleados(string nombreArchivo) {
    ifstream archivo;
    DatosEmpleado e;
    int contador = 0;
    int activos = 0;
    
    archivo.open(nombreArchivo, ios::binary);
    
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados o error al abrir el archivo." << endl;
        return;
    }
    
    cout << endl << "=== LISTA DE TODOS LOS EMPLEADOS ===" << endl;
    
    while (archivo.read((char*)&e, sizeof(DatosEmpleado))) {
        contador++;
        if (!e.eliminado) {
            activos++;
            cout << endl << "EMPLEADO " << activos << ":" << endl;
            cout << "CI: " << e.CI_Empleado << endl;
            cout << "Nombre: " << e.nombre << " " << e.apellido << endl;
            cout << "Rango: ";
            if (e.rango == 1) {
                cout << "Vendedor";
            }
            else if (e.rango == 2) {
                cout << "Cajero";
            }
            else {
                cout << "Desconocido";
            }
            cout << ")" << endl;
            cout << "--------------------------------------" << endl;
        }
    }
    
    archivo.close();
    
    if (contador == 0) {
        cout << "No hay empleados registrados en el archivo." << endl;
        system("pause");
    } else {
        cout << endl << "Total de empleados registrados: " << contador << endl;
        cout << "Empleados activos: " << activos << endl;
        cout << "Empleados eliminados (no mostrados): " << (contador - activos) << endl;
        system("pause");
    }
}


void eliminarEmpleado(string nombreArchivo) {
    char ciEliminar[10];
    fstream archivo;
    DatosEmpleado e;
    bool encontrado = false;
    
    cout << endl << "=== ELIMINAR EMPLEADO ===" << endl;
    cout << "Ingrese el CI del empleado a eliminar: ";
    cin.getline(ciEliminar, 10);
    
    archivo.open(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados para eliminar." << endl;
        system("pause");
        return;
    }
    
    while (archivo.read((char*)&e, sizeof(DatosEmpleado)) && !encontrado) {
        if (strcmp(e.CI_Empleado, ciEliminar) == 0 && !e.eliminado) {
            encontrado = true;
            cout << "Empleado encontrado: " << e.nombre << " " << e.apellido << endl;
            cout << "Esta seguro de marcar como eliminado? (s/n): ";
            char confirmacion;
            cin >> confirmacion;
            cin.ignore();
            
            if (confirmacion == 's' || confirmacion == 'S') {
                e.eliminado = true;
                archivo.seekp(-sizeof(DatosEmpleado), ios::cur);
                archivo.write((char*)&e, sizeof(DatosEmpleado));
                archivo.flush();
                cout << "Empleado marcado como ELIMINADO correctamente." << endl;
            } else {
                cout << "Operacion cancelada." << endl;
            }
        }
    }
    
    archivo.close();

    
    if (!encontrado) {
        cout << "No se encontro empleado activo con CI: " << ciEliminar << endl;
    }
    system("pause");
}

void modificarEmpleado(string nombreArchivo) {
    char ciModificar[10];
    fstream archivo;
    DatosEmpleado e;
    bool encontrado = false;
    int opcion;
    
    cout << endl << "=== MODIFICAR EMPLEADO ===" << endl;
    cout << "Ingrese el CI del empleado a modificar: ";
    cin.getline(ciModificar, 10);
    
    archivo.open(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados para modificar" << endl;
        return;
    }
    
    while (archivo.read((char*)&e, sizeof(DatosEmpleado)) && !encontrado) {
        if (strcmp(e.CI_Empleado, ciModificar) == 0 && !e.eliminado) {
            encontrado = true;
            
            cout << endl << "EMPLEADO ENCONTRADO:" << endl;
            cout << "CI: " << e.CI_Empleado << endl;
            cout << "Nombre: " << e.nombre << " " << e.apellido << endl;
            cout << "Rango: " << e.rango << endl;
            
            do {
                cout << endl << "Que dato desea modificar?" << endl;
                cout << "1. Nombre" << endl;
                cout << "2. Apellido" << endl;
                cout << "3. Rango" << endl;
                cout << "4. Salir de modificacion" << endl;
                cout << "Seleccione una opcion: ";
                cin >> opcion;
                cin.ignore();
                
                switch(opcion) {
                    case 1:
                        cout << "Ingrese el nuevo nombre: ";
                        cin.getline(e.nombre, 30);
                        cout << "Nombre actualizado correctamente." << endl;
                        break;
                    case 2:
                        cout << "Ingrese el nuevo apellido: ";
                        cin.getline(e.apellido, 30);
                        cout << "Apellido actualizado correctamente." << endl;
                        break;
                    case 3:
                        cout << "Ingrese el nuevo rango:" << endl;
                        cout << "1. Vendedor" << endl;
                        cout << "2. Cajero" << endl;
                        cout << "Seleccione: ";
                        cin >> e.rango;
                        cin.ignore();
                        cout << "Rango actualizado correctamente." << endl;
                        break;
                    case 4:
                        cout << "Saliendo de la modificacion..." << endl;
                        break;
                    default:
                        cout << "Opcion no valida. Intente de nuevo." << endl;
                }
                
            } while (opcion != 4);
            
            archivo.seekp(-sizeof(DatosEmpleado), ios::cur);
            archivo.write((char*)&e, sizeof(DatosEmpleado));
            archivo.flush();
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << "No se encontro empleado activo con CI: " << ciModificar << endl;
    } else {
        cout << "Empleado modificado correctamente." << endl;
    }
    system("pause");
}

void buscarEmpleadoPorCI(string nombreArchivo) {
    char ciBuscar[10];
    ifstream archivo;
    DatosEmpleado e;
    bool encontrado = false;
    
    cout << endl << "=== BUSCAR EMPLEADO POR CI ===" << endl;
    cout << "Ingrese el CI del empleado a buscar: ";
    cin.getline(ciBuscar, 10);
    
    archivo.open(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        cout << "No hay empleados registrados." << endl;
        return;
    }
    
    while (archivo.read((char*)&e, sizeof(DatosEmpleado))) {
        if (strcmp(e.CI_Empleado, ciBuscar) == 0 && !e.eliminado) {
            encontrado = true;
            cout << endl << "EMPLEADO ENCONTRADO:" << endl;
            cout << "CI: " << e.CI_Empleado << endl;
            cout << "Nombre: " << e.nombre << " " << e.apellido << endl;
            cout << "Rango: " << e.rango << " (";
            if (e.rango == 1) cout << "Vendedor";
            else if (e.rango == 2) cout << "Cajero";
            else cout << "Desconocido";
            cout << ")" << endl;
            break;
        }
    }
    
    archivo.close();
    
    if (!encontrado) {
        cout << "No se encontró empleado activo con CI: " << ciBuscar << endl;
    }
    system("pause");
}

void reporteEmpleadoMasVende(string nombreArchivoVentas, string nombreArchivoEmpleados) {
    ifstream archivo_ventas;
    ventaProducto venta;
    
    // Vectores para almacenar empleados y sus ventas
    vector<string> empleados;
    vector<float> ventasEmpleados;
    
    archivo_ventas.open(nombreArchivoVentas, ios::binary);
    if (!archivo_ventas.is_open()) {
        cout << "No hay ventas registradas para generar el reporte." << endl;
        return;
    }
    
    // Procesar todas las ventas
    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto))) {
        if (!venta.eliminada) {
            float subtotal = venta.cantidad * venta.precioUnitario;
            
            // Buscar si el empleado ya esta en la lista
            bool encontrado = false;
            for (size_t i = 0; i < empleados.size(); i++) {
                if (empleados[i] == venta.CI_Empleado) {
                    ventasEmpleados[i] += subtotal;
                    encontrado = true;
                    break;
                }
            }
            
            // Si no esta, agregarlo
            if (!encontrado) {
                empleados.push_back(venta.CI_Empleado);
                ventasEmpleados.push_back(subtotal);
            }
        }
    }
    archivo_ventas.close();
    
    if (empleados.empty()) {
        cout << "No hay empleados con ventas registradas." << endl;
        system("pause");
        return;
    }
    
    // Ordenar empleados por ventas (ordenamiento simple burbuja)
    for (size_t i = 0; i < empleados.size() - 1; i++) {
        for (size_t j = i + 1; j < empleados.size(); j++) {
            if (ventasEmpleados[j] > ventasEmpleados[i]) {
                // Intercambiar empleados
                string tempEmp = empleados[i];
                empleados[i] = empleados[j];
                empleados[j] = tempEmp;
                
                // Intercambiar ventas
                float tempVenta = ventasEmpleados[i];
                ventasEmpleados[i] = ventasEmpleados[j];
                ventasEmpleados[j] = tempVenta;
            }
        }
    }
    
    cout << endl << "=== REPORTE: EMPLEADO QUE MÁS VENDE ===" << endl;
    
    // Mostrar el empleado top
    if (!empleados.empty()) {
        cout << "Empleado con mas ventas:" << endl;
        cout << "CI: " << empleados[0] << endl;
        cout << "Total vendido: $" << ventasEmpleados[0] << endl;
        
        // Buscar informacion del empleado en el archivo
        ifstream archivoEmpleados(nombreArchivoEmpleados, ios::binary);
        DatosEmpleado empleado;
        bool encontrado = false;
        
        while (archivoEmpleados.read((char*)&empleado, sizeof(DatosEmpleado))) {
            if (strcmp(empleado.CI_Empleado, empleados[0].c_str()) == 0 && !empleado.eliminado) {
                encontrado = true;
                cout << "Nombre: " << empleado.nombre << " " << empleado.apellido << endl;
                cout << "Rango: ";
                if (empleado.rango == 1) cout << "Vendedor";
                else if (empleado.rango == 2) cout << "Cajero";
                else cout << empleado.rango;
                cout << endl;
                break;
            }
        }
        archivoEmpleados.close();
        
        if (!encontrado) {
            cout << "Empleado no encontrado en el registro actual." << endl;
        }
    }
    
    // Mostrar ranking completo
    cout << endl << "RANKING COMPLETO DE EMPLEADOS:" << endl;
    cout << "==================================" << endl;
    
    for (size_t i = 0; i < empleados.size(); i++) {
        cout << i+1 << ". CI: " << empleados[i] << " - Ventas: $" << ventasEmpleados[i] << endl;
    }
    system("pause");
}


void reporteMesMasVentas(string nombreArchivoVentas) {
    ifstream archivo_ventas;
    ventaProducto venta;
    
    // Array para meses (1-12)
    string nombresMeses[] = {"", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio","Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    
    // Contadores para cada mes
    float ventasPorMes[13] = {0}; // Indice 0 no usado
    int cantidadPorMes[13] = {0};
    
    archivo_ventas.open(nombreArchivoVentas, ios::binary);
    if (!archivo_ventas.is_open()) {
        cout << "No hay ventas registradas para generar el reporte." << endl;
        system("pause");
        return;
    }
    
    // Calcular ventas por mes
    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto))) {
        if (!venta.eliminada) {
            int mes = venta.fechaDeVenta.mes;
            if (mes >= 1 && mes <= 12) {
                float subtotal = venta.cantidad * venta.precioUnitario;
                ventasPorMes[mes] += subtotal;
                cantidadPorMes[mes] += venta.cantidad;
            }
        }
    }
    archivo_ventas.close();
    
    // Encontrar mes con mas ventas
    int mesTop = 0;
    float maxVentasMes = 0;
    int maxCantidad = 0;
    
    for (int mes = 1; mes <= 12; mes++) {
        if (ventasPorMes[mes] > maxVentasMes) {
            maxVentasMes = ventasPorMes[mes];
            maxCantidad = cantidadPorMes[mes];
            mesTop = mes;
        }
    }
    
    cout << endl << "=== REPORTE: MES CON MAS VENTAS ===" << endl;
    
    if (mesTop > 0) {
        cout << "Mes con mas ventas: " << nombresMeses[mesTop] << endl;
        cout << "Ventas totales del mes: $" << maxVentasMes << endl;
        cout << "Cantidad de productos vendidos: " << maxCantidad << endl;
    } else {
        cout << "No se encontraron ventas registradas." << endl;
    }
    
    // Mostrar ventas por todos los meses
    cout << endl << "VENTAS POR MES:" << endl;
    cout << "==================" << endl;
    
    float totalAnual = 0;
    int totalProductosAnual = 0;
    
    for (int mes = 1; mes <= 12; mes++) {
        if (ventasPorMes[mes] > 0) {
            cout << nombresMeses[mes] << ": $" << ventasPorMes[mes] 
                 << " (" << cantidadPorMes[mes] << " productos)" << endl;
            totalAnual += ventasPorMes[mes];
            totalProductosAnual += cantidadPorMes[mes];
        }
    }
    
    cout << endl << "Total anual: $" << totalAnual << endl;
    cout << "Total productos vendidos anual: " << totalProductosAnual << endl;
    system("pause");
}

void menuEmpleados(string nombreArchivoEmpleados, string nombreArchivoVentas) {
    int opcion;
    do {
        system("cls");
        cout << endl << "=== MENÚ DE GESTIÓN DE EMPLEADOS ===" << endl;
        cout << "1. Registrar nuevo empleado" << endl;
        cout << "2. Mostrar todos los empleados (solo activos)" << endl;
        cout << "3. Buscar empleado por CI" << endl;
        cout << "4. Modificar empleado por CI" << endl;
        cout << "5. Eliminar empleado por CI (eliminacion logica)" << endl;
        cout << "6. Reporte: Empleado que mas vende" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();
        
        switch(opcion) {
            case 1:
                llenarDatosEmpleado(nombreArchivoEmpleados);
                break;
            case 2:
                mostrarTodosEmpleados(nombreArchivoEmpleados);
                break;
            case 3:
                buscarEmpleadoPorCI(nombreArchivoEmpleados);
                break;
            case 4:
                modificarEmpleado(nombreArchivoEmpleados);
                break;
            case 5:
                eliminarEmpleado(nombreArchivoEmpleados);
                break;
            case 6:
                reporteEmpleadoMasVende(nombreArchivoVentas, nombreArchivoEmpleados);
                break;
            case 0:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcion != 0);
}



// ------------------------------------------------ PRODUCTOS -----------------------------------------------------------------------------------

void categoriasProductoMenu() {
    cout << "\tCATEGORÍAS\n";
    cout << "\t\t1. Celular\n";
    cout << "\t\t2. Tablet\n";
    cout << "\t\t3. Laptop\n";
    cout << "\t\t4. Televisor\n";
    cout << "\t\t5. Monitor\n";
    cout << "\t\t6. Parlante\n";
    cout << "\t\t7. Proyector\n";
    cout << "\t\t8. Lavadora\n";
    cout << "\t\t9. Refrigerador\n";
}

DatosProducto insertarDatosProducto() {
    DatosProducto producto;

    cout << "Código: ";
    cin >> producto.codigo;
    cin.ignore();
    cout << "Modelo: ";
    cin.getline(producto.modelo, 30);
    cout << "Categoría: ";
    categoriasProductoMenu();
    cin >> producto.categoria;
    producto.categoria -= 1; // Rebajar en un número la categoría para que sea acorde al vector
    cout << "Precio de venta: ";
    cin >> producto.precioVenta;
    cout << "Stock: ";
    cin >> producto.stock;
    producto.eliminado = false; // Al agregar un producto, se lo coloca automáticamente como No eliminado
    
    return producto;
}

void agregarProducto (string nombreArchivo) {
    ofstream archivo;
    DatosProducto producto = insertarDatosProducto();
    archivo.open(nombreArchivo, ios::binary | ios::app); // se añaden datos al archivo
    if (archivo.good()) {
        archivo.write((char*)(&producto), sizeof(DatosProducto));
        cout << "Producto añadido con exito\n";
    } else {
        cout << "Error al añadir producto\n";
        system("pause");
        return;
    }
    archivo.close();
    system("pause");
}

void mostrarListaProductosPantalla(string nombreArchivo) {
    ifstream archivo;
    DatosProducto producto;
    system("cls");

    archivo.open(nombreArchivo, ios::binary);
    if (archivo.good()) {
        cout << "============ LISTA DE PRODUCTOS DISPONIBLES ===================\n";
        while(archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (producto.eliminado==false) {
                cout << "\tCódigo: " << producto.codigo << endl;
                cout << "\tModelo: " << producto.modelo << endl;
                cout << "\tCategoría: " << categoriasProductoVector[producto.categoria] << endl;
                cout << "\tPrecio de venta: " << producto.precioVenta << endl;
                cout << "\tStock: " << producto.stock << endl;
                cout << "--------------------------------------------------------\n";
            }
        }
        archivo.close();
        system("pause");

    } else {
        cout << "Error al abrir el archivo de productos\n";
        system("pause");
        return;
    }
}

void ModificarProducto(string nombreArchivo) { 
    fstream archivo; 
    DatosProducto producto;
    int codigoBuscado; 
    bool encontrado = false;
    char confirmacion;

    system("cls");
    cout << "Ingrese el código del producto a modificar: ";
    cin >> codigoBuscado;

    //Abrir el archivo
    archivo.open(nombreArchivo, ios::in | ios::out | ios::binary);
    if (archivo.good()) {
        // Buscar el producto
        while (archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (codigoBuscado==producto.codigo && encontrado==false) {
                cout << "=== PRODUCTO A MODIFICAR ===\n";
                cout << "\tCódigo: " << producto.codigo << endl;
                cout << "\tModelo: " << producto.modelo << endl;
                cout << "\tCategoría: " << categoriasProductoVector[producto.categoria] << endl;
                cout << "\tPrecio de venta: " << producto.precioVenta << endl;
                cout << "\tStock: " << producto.stock << endl;
                encontrado = true;

                cout << "¿Desea modificar este producto? (s/n): ";
                cin >> confirmacion;
                if (confirmacion == 's') {
                    archivo.seekp(-sizeof(DatosProducto), ios::cur);
                    cin.ignore();
                    cout << "NUEVOS DATOS DEL PRODUCTO\n";
                    cout << "Modelo: ";
                    cin.getline(producto.modelo, 30);
                    cout << "Categoría: ";
                    categoriasProductoMenu();
                    cin >> producto.categoria;
                    producto.categoria-=1;
                    cout << "Precio de venta: ";
                    cin >> producto.precioVenta;
                    cout << "Stock: ";
                    cin >> producto.stock;
                    archivo.write((char*)(&producto), sizeof(DatosProducto));
                } else {
                    archivo.close();
                    return;
                }
            }
        }
        if (encontrado == false) {
            cout << "No se pudo encontrar el producto.\n";
        }
    } else {
        cout << "Error al eliminar producto\n";
        system("pause");
        return;
    }
    archivo.close();
    system("pause");
}

void eliminarProducto(string nombreArchivo) {
    fstream archivo; // leer y modificar
    DatosProducto producto;
    int codigoBuscado; // se buscará por código
    bool encontrado = false;
    char confirmacion; // se hará una pregunta de confirmación para eliminar el contacto

    cout << "Ingrese el código del producto a eliminar: ";
    cin >> codigoBuscado;

    //Abrir el archivo
    archivo.open(nombreArchivo, ios::in | ios::out | ios::binary);
    if (archivo.good()) {
        // Buscar el producto
        while (archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (codigoBuscado==producto.codigo && encontrado==false) {
                cout << "=== PRODUCTO A ELIMINAR ===\n";
                cout << "\tCódigo: " << producto.codigo << endl;
                cout << "\tModelo: " << producto.modelo << endl;
                cout << "\tCategoría: " << producto.categoria << endl;
                cout << "\tPrecio de venta: " << producto.precioVenta << endl;
                cout << "\tStock: " << producto.stock << endl;
                encontrado = true;

                cout << "¿Desea eliminar este producto? (s/n): ";
                cin >> confirmacion;
                if (confirmacion == 's') {
                    producto.eliminado = true;
                    archivo.seekp(-sizeof(DatosProducto), ios::cur);
                    archivo.write((char*)(&producto), sizeof(DatosProducto));
                    cout << "Producto eliminado\n";
                } else {
                    archivo.close();
                    system("pause");
                    return;
                }
            }
        }
        if (encontrado == false) {
            cout << "No se pudo encontrar el producto.\n";
        }
    } else {
        cout << "Error al eliminar producto\n";
        system("pause");
        return;
    }
    archivo.close();
    system("pause");
}

void buscarProductoPorCodigo(string nombreArchivo) {
    ifstream archivo;
    DatosProducto producto;
    bool encontrado = false;
    int codigoBuscado;

    cout << "Ingrese el código del producto que busca: ";
    cin >> codigoBuscado;

    archivo.open(nombreArchivo, ios::binary);
    if (archivo.good()) {
        while (archivo.read((char*)(&producto), sizeof(DatosProducto)) && encontrado==false) {
            if (producto.codigo == codigoBuscado && producto.eliminado==false) {
                cout << "== PRODUCTO ENCONTRADO ==\n";
                cout << "Código: " << producto.codigo << endl;
                cout << "Modelo: " << producto.modelo << endl;
                cout << "Categoría: " << categoriasProductoVector[producto.categoria] << endl;
                cout << "Precio: " << producto.precioVenta << endl;
                cout << "Stock: " << producto.stock << endl;
                cout << "=========================\n";
                encontrado = true;
            }
        }
        if (encontrado==false) {
            cout << "No se pudo encontrar un producto con ese código: ";
            system("pause");
            return;
        }
        system("pause");
        archivo.close();
    } else {
        cout << "Error al buscar el producto\n";
        system("pause");
        return;
    }
}

void productosBajoStock(string nombreArchivo) {
    ifstream archivo;
    DatosProducto producto;

    archivo.open(nombreArchivo, ios::binary);
    if (archivo.good()) {
        cout << "====== PRODUCTOS DE BAJO STOCK ======\n";
        while (archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (producto.stock<5) {
                cout << "Código: " << producto.codigo << endl;
                cout << "Modelo: " << producto.modelo << endl;
                cout << "Categoría: " << producto.categoria << endl;
                cout << "Precio: " << producto.precioVenta << endl;
                cout << "Stock: " << producto.stock << endl;
                cout << "=========================\n";
            }
        }
        system("pause");
        archivo.close();
    } else {
        cout << "Error al buscar productos\n";
        system("pause");
        return;
    }
}

void menuABM_Productos(string nombreArchivoBin) {
    int opcion;
    do {
        system("cls");
        cout << "== MENÚ PRODUCTOS ==\n";
        cout << "\t1. Agregar producto\n";
        cout << "\t2. Modificar producto\n";
        cout << "\t3. Eliminar producto\n";
        cout << "\t4. Mostrar lista de productos disponibles\n";
        cout << "\t5. Buscar producto por código\n";
        cout << "\t6. Reporte: productos con bajo stock (menos de 5 unidades)\n";
        cout << "\t0. Volver\n";
        cout << "--> ";
        cin >> opcion;
        if (opcion==1) {
            agregarProducto(nombreArchivoBin);
        } else if (opcion==2) {
            ModificarProducto(nombreArchivoBin);
        } else if (opcion==3) {
            eliminarProducto(nombreArchivoBin);
        } else if(opcion==4) {
            mostrarListaProductosPantalla(nombreArchivoBin);
        } else if (opcion==5) {
            buscarProductoPorCodigo(nombreArchivoBin);
        } else if (opcion==6) {
            productosBajoStock(nombreArchivoBin);
        }else if (opcion==0) {
            cout << "Volviendo al menú principal...\n";
        }
    } while (opcion!=0);
}

// ----------------------------------------------------------- REPARACIONES ----------------------------------------------------------------------------------


Fecha encontrarFechaFactura (string nombreArchivoFacturas, int numeroFacturaBuscado) {
    ifstream archivoFacturas;
    datosFactura factura;
    bool encontrado = false;
    Fecha fechaADevolver = {0,0,0};

    archivoFacturas.open(nombreArchivoFacturas, ios::binary);
    if (archivoFacturas.good()) {
        while(archivoFacturas.read((char*)(&factura), sizeof(datosFactura)) && encontrado==false) {
            if (factura.numeroFactura==numeroFacturaBuscado && factura.anulada==false) {
                fechaADevolver = factura.fecha_emision_factura;
                encontrado = true;
            }
        }
        if (encontrado==false) {
            cout << "No se pudo encontrar la factura de la compra. No se puede ofrecer ningún descuento.\n";
            system("pause");
        }
        archivoFacturas.close();
    } else {
        cout << "Error al abrir el archivo de facturas\n";
        system("pause");
    }
    return fechaADevolver;
}

bool anioEsBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

Reparaciones DatosReparacion(char ciClient[], int codigoProd, int numFactura, string nombreArchivoFacturas) {
    Reparaciones reparacion;
    Fecha datosFechaTemporal;
    Fecha fechaFactura;
    ifstream archivoFacturas;
    datosFactura factura;
    bool anioBisiesto;
    int diasDeDiferenciaEntreFechas = 0;


    strcpy(reparacion.CI_cliente,ciClient);
    reparacion.codigoProducto = codigoProd;
    reparacion.numeroFactura = numFactura;
    cout << "Descripción de los daños(99 caracteres): ";
    cin.ignore();
    cin.getline(reparacion.descripcion, 100);
    cout << "Fecha: \n";
    cout << "\tDía: ";
    cin >> datosFechaTemporal.dia;
    cout << "\tMes: ";
    cin >> datosFechaTemporal.mes;
    cout << "\tAño: ";
    cin >> datosFechaTemporal.anio;
    reparacion.fechaReparacion = datosFechaTemporal;

    fechaFactura = encontrarFechaFactura(nombreArchivoFacturas, numFactura);
    if (fechaFactura.anio==0) {
        cout << "No se encontró la factura\n";
    }
    // sumar años incompletos
    for (int i=fechaFactura.mes; i<13; i++) {
        if (anioEsBisiesto(fechaFactura.anio) && fechaFactura.mes<=2) {
            diasDeDiferenciaEntreFechas += diasMeses[i];
        } else {
            diasDeDiferenciaEntreFechas += diasMeses[i];
        }
    }
    // sumar años completos
    for (int i=fechaFactura.anio+1; i<reparacion.fechaReparacion.anio; i++) {
        if (anioEsBisiesto(i)) {
            diasDeDiferenciaEntreFechas += 366;
        } else {
            diasDeDiferenciaEntreFechas += 365;
        }
    }
    if (diasDeDiferenciaEntreFechas > 365) {
        cout << "La garantía ha expirado\nIntroduzca el precio de la reparación: ";
        cin >> reparacion.precioReparacion;
    } else {
        cout << "La garantía continúa vigente, la repración no tendrá costo\n";
        reparacion.precioReparacion = 0;
    }

    return reparacion;
}

void agregarDatosReparacion(string nombreArchivoReparacion, string nombreArchivoFacturas, string NombreArchivoCliente, string nombreArchivoProductos) {
    ofstream archivoReparacion;
    ifstream archivoClientes;
    ifstream archivoFacturas;
    ifstream archivoProductos;
    Reparaciones repararacion;
    DatosCliente cliente;
    datosFactura factura;
    DatosProducto producto;
    char CIClienteBuscado[10];
    bool encontradoCliente=false;
    int numFacturaBuscada;
    bool encontradoFactura=false;
    int codigoProductoBuscado;
    bool encontradoCodigo=false;

    cout << "DATOS DE REPARACIÓN\n";
    archivoClientes.open(NombreArchivoCliente, ios::binary);
    if (archivoClientes.good()) {
        cin.ignore();
        cout << "CI del Cliente: ";
        cin.getline(CIClienteBuscado,10);
        while (archivoClientes.read((char*)(&cliente), sizeof(DatosCliente)) && encontradoCliente==false) {
            if (strcmp(cliente.CI_Cliente,CIClienteBuscado)==0 &&cliente.eliminado==false) {
                encontradoCliente = true;
            }
        }
        if (encontradoCliente==false) {
            cout << "No se ha encontrado a un cliente con ese CI\n";
            archivoClientes.close();
            return;
        }
    } else {
        cout << "Error al abrir el archivo de clientes\n";
        return;
    }
archivoClientes.close();

    archivoFacturas.open(nombreArchivoFacturas, ios::binary);
    if (archivoFacturas.good()) {
        cout << "Número de Factura: ";
        cin >> numFacturaBuscada;
        while (archivoFacturas.read((char*)(&factura), sizeof(datosFactura)) && encontradoFactura==false) {
            if (numFacturaBuscada==factura.numeroFactura && factura.anulada==false) {
                encontradoFactura = true;
            }
        }
        if (encontradoFactura==false) {
            cout << "No se ha encontrado la factura\n";
            return;
        }
    } else {
        cout << "Error al abrir el archivo de facturas\n";
        return;
    }
    archivoProductos.open(nombreArchivoProductos, ios::binary);
    if (archivoProductos.good()) {
        cout << "Código del Producto: ";
        cin >> codigoProductoBuscado;
        while (archivoProductos.read((char*)(&producto), sizeof(DatosProducto)) && encontradoCodigo==false) {
            if (codigoProductoBuscado==producto.codigo) {
                encontradoCodigo = true;
            }
        }
        if (encontradoCodigo==false) {
            cout << "El producto no está disponible\n";
            archivoProductos.close();
            return;
        }
    } else {
        cout << "Error al abrir el archivo de productos\n";
        return;
    }
    archivoProductos.close();

    archivoReparacion.open(nombreArchivoReparacion, ios::binary | ios::app);
    if (archivoReparacion.good()) {
        repararacion = DatosReparacion(CIClienteBuscado,codigoProductoBuscado,numFacturaBuscada,nombreArchivoFacturas);
        archivoReparacion.write((char*)(&repararacion), sizeof(Reparaciones));
        archivoReparacion.close();
        cout << "Datos de la reparación agregados con éxito.";
        system("pause");
    } else {
        cout << "Error al agregar los datos de la reparación";
        system("pause");
        return;
    }
}

void encontrarReparacion(string nombreArchivo) {
    fstream archivo;
    Reparaciones reparacion;
    bool encontrado = false;
    char ciBuscado[10];
    int codigoProductoBuscado;
    int numeroFacturaBuscada;

    archivo.open(nombreArchivo, ios::in | ios::out | ios::binary);
    if (archivo.good()) {
        cout << "== DATOS PARA ENCONTRAR LA REPARACIÓN ==\n";
        cout << "\tCI: ";
        cin.ignore();
        cin.getline(ciBuscado,10);
        cout << "\tCódigo del Producto: ";
        cin >> codigoProductoBuscado;
        cout << "\tNúmero de la Factura: ";
        cin >> numeroFacturaBuscada;
        while(archivo.read((char*)(&reparacion), sizeof(Reparaciones)) && encontrado==false) {
            if ((strcmp(ciBuscado,reparacion.CI_cliente)==0) && codigoProductoBuscado==reparacion.codigoProducto && 
            numeroFacturaBuscada==reparacion.numeroFactura) {
                system("cls");
                cout << "REGISTRO DE REPARACIÓN ENCONTRADO\n";
                cout << "\tCliente: " << reparacion.CI_cliente << endl;
                cout << "\tCodigo del Producto: " << reparacion.codigoProducto << endl;
                cout << "\tNúmero de Factura: " << reparacion.numeroFactura << endl;
                cout << "\tFecha: " << reparacion.fechaReparacion.dia << "/" << reparacion.fechaReparacion.mes << "/" << reparacion.fechaReparacion.anio << endl;
                cout << "\tPrecio de la Reparación: " << reparacion.precioReparacion << endl;
                cout << "\tDescripción: " << reparacion.descripcion << endl;
                cout << "-----------------------------------------------------\n";
                encontrado = true;
            }
        }
        if (encontrado==false) {
            cout << "Reparación no encontrada";
        }
    } else {
        cout << "Error al encontrar registro de reparación los datos de la reparación";
        system("pause");
        return;
    }
}

void mostrarReparaciones (string nombreArchivo) {
    fstream archivo;
    Reparaciones reparacion;

    archivo.open(nombreArchivo, ios::in | ios::out | ios::binary);
    if (archivo.good()) {
        cout << "LISTA DE REPARACIONES\n";
        while(archivo.read((char*)(&reparacion), sizeof(Reparaciones))) {
            
                cout << "\tCliente: " << reparacion.CI_cliente << endl;
                cout << "\tCodigo del Producto: " << reparacion.codigoProducto << endl;
                cout << "\tNúmero de Factura: " << reparacion.numeroFactura << endl;
                cout << "\tFecha: " << reparacion.fechaReparacion.dia << "/" << reparacion.fechaReparacion.mes << "/" << reparacion.fechaReparacion.anio << endl;
                cout << "\tPrecio de la Reparación: " << reparacion.precioReparacion << endl;
                cout << "\tDescripción: " << reparacion.descripcion << endl;
                cout << "-----------------------------------------------------\n"; 
        }
    } else {
        cout << "Error al mostrar reparaciones\n";
        system("pause");
        return;
    }
    system("pause");
}

void modificarDatosReparacion(string nombreArchivo) {
    fstream archivo;
    Reparaciones reparacion;
    bool encontrado = false;
    char ciBuscado[10];
    int codigoProductoBuscado;
    int numeroFacturaBuscada;

    archivo.open(nombreArchivo, ios::in | ios::out | ios::binary);
    if (archivo.good()) {
        cout << "== DATOS PARA ENCONTRAR LA REPARACIÓN ==\n";
        cout << "\tCI: ";
        cin.ignore();
        cin.getline(ciBuscado,10);
        cout << "\tCódigo del Producto: ";
        cin >> codigoProductoBuscado;
        cout << "\tNúmero de la Factura: ";
        cin >> numeroFacturaBuscada;
        while(archivo.read((char*)(&reparacion), sizeof(Reparaciones)) && encontrado==false) {
            if ((strcmp(ciBuscado,reparacion.CI_cliente)==0) && codigoProductoBuscado==reparacion.codigoProducto && 
            numeroFacturaBuscada==reparacion.numeroFactura) {
                system("cls");
                cout << "REGISTRO DE REPARACIÓN ENCONTRADO\n";
                cout << "\tCliente: " << reparacion.CI_cliente << endl;
                cout << "\tCodigo del Producto: " << reparacion.codigoProducto << endl;
                cout << "\tNúmero de Factura: " << reparacion.numeroFactura << endl;
                cout << "\tFecha: " << reparacion.fechaReparacion.dia << "/" << reparacion.fechaReparacion.mes << "/" << reparacion.fechaReparacion.anio << endl;
                cout << "\tPrecio de la Reparación: " << reparacion.precioReparacion << endl;
                cout << "\tDescripcion: " << reparacion.descripcion << endl;
                cout << "-----------------------------------------------------\n";
                encontrado = true;
                cout << "DATOS NUEVOS\n";
                archivo.seekp(-sizeof(Reparaciones), ios::cur);
                cout << "Descripción: ";
                cin.getline(reparacion.descripcion,100);
                archivo.write((char*)(&reparacion), sizeof(Reparaciones));
                archivo.close();
                cout << "Registro de Reparación modificado con éxito\n";
            }
        }
        if (encontrado==false) {
            cout << "Reparación no encontrada";
        }
    } else {
        cout << "Error al modificar los datos de la reparación";
        system("pause");
        return;
    }
}

//--------Reportes Reparaciones--------------
void ReporteMensualReparacionesPantalla (string nombreArchivoBinReparaciones, string nombreArchivoBinClientes, string nombreArchivoBinFacturas, string nombreArchivoBinProductos) {
    ifstream archivoBinRepararaciones; //leer
    ifstream archivoBinClientes;
    ifstream archivoBinFacturas;
    ifstream archivoBinProductos;
    Reparaciones reparacion;
    DatosCliente cliente;
    datosFactura factura;
    DatosProducto producto;
    int mesBuscado;
    int anioBuscado;
    bool unaReparacion = false;

    vector<DatosCliente> clientesTemporal;
    vector<DatosProducto> productoTemporal;
    vector<datosFactura> facturaTemporal;

    archivoBinClientes.open(nombreArchivoBinClientes, ios::binary);
    if (archivoBinClientes.good()) {
        while(archivoBinClientes.read((char*)(&cliente), sizeof(DatosCliente))) {
            clientesTemporal.push_back(cliente);
        }
    } else {
        cout << "No se pudo abrir el archivo de clientes\n";
        return;
    }
    archivoBinClientes.close();

    archivoBinProductos.open(nombreArchivoBinProductos, ios::binary);
    if (archivoBinProductos.good()) {
        while(archivoBinProductos.read((char*)(&producto), sizeof(DatosProducto))) {
            productoTemporal.push_back(producto);
        }
    } else {
        cout << "No se pudo abrir el archivo de productos\n";
        return;
    }
    archivoBinProductos.close();

    archivoBinFacturas.open(nombreArchivoBinFacturas, ios::binary);
    if (archivoBinFacturas.good()) {
        while(archivoBinFacturas.read((char*)(&factura), sizeof(datosFactura))) {
            facturaTemporal.push_back(factura);
        }
    } else {
        cout << "No se pudo abrir el archivo de facturas\n";
        return;
    }
    archivoBinFacturas.close();

    archivoBinRepararaciones.open(nombreArchivoBinReparaciones, ios::binary);
    if (archivoBinRepararaciones.good()) {
        cout << "REPORTE MENSUAL DE REPARACIONES\n";
        cout << "Mes del reporte: ";
        cin >> mesBuscado;
        cout << "Año del reporte: ";
        cin >> anioBuscado;
        cout << "===== REPARACIONES DEL MES DE " << meses[mesBuscado] << " DE " << anioBuscado << " =====\n";    
        while(archivoBinRepararaciones.read((char*)(&reparacion), sizeof(Reparaciones))) {
            if (reparacion.fechaReparacion.mes==mesBuscado && reparacion.fechaReparacion.anio==anioBuscado) {
                unaReparacion = true;
                cout << "--------------------------------------------------\n";
                cout << "Fecha: " << reparacion.fechaReparacion.dia << "/" << reparacion.fechaReparacion.mes << "/" << reparacion.fechaReparacion.anio << endl;
                for (int i=0; i<clientesTemporal.size(); i++) {
                    if (strcmp(reparacion.CI_cliente, clientesTemporal[i].CI_Cliente)==0) {
                        cout << "DATOS DEL CLIENTE\n";
                        cout << "CI: " << clientesTemporal[i].CI_Cliente;
                        cout << "Nombres y Apellidos: " << clientesTemporal[i].nombre << " " << clientesTemporal[i].apellido << endl;
                        cout << "Tel. y Correo: " << clientesTemporal[i].telefono << " / " << clientesTemporal[i].correo << endl;
                    }
                }

                for (int i=0; i<productoTemporal.size(); i++) {
                    if (productoTemporal[i].codigo==reparacion.codigoProducto) {
                        cout << "DATOS DEL PRODUCTO\n";
                        cout << "Código: " << productoTemporal[i].codigo;
                        cout << "Modelo y categoría: " << productoTemporal[i].modelo << ", " << categoriasProductoVector[productoTemporal[i].categoria] << endl;
                        cout << "Precio de Venta: " << productoTemporal[i].precioVenta << endl;
                        if (productoTemporal[i].eliminado) {
                                cout << "NOTA: A fecha de este reporte, este producto no se encuentra disponible.\n";
                            }
                    }
                }

                for (int i=0; i<facturaTemporal.size(); i++) {
                    if (reparacion.numeroFactura==facturaTemporal[i].numeroFactura) {
                        cout << "DATOS DE LA FACTURA\n";
                        cout << "Número de Factura: "  << reparacion.numeroFactura << endl;
                        cout << "Fecha de emisión: " << facturaTemporal[i].fecha_emision_factura.dia << "/" << facturaTemporal[i].fecha_emision_factura.mes << "/" << facturaTemporal[i].fecha_emision_factura.anio << endl;
                    }
                }

                cout << "Precio de la Reparación: " << reparacion.precioReparacion << endl;
                cout << "Motivo: " << reparacion.descripcion << endl;
            }
        }
        if (unaReparacion==false) {
            cout << "No se encontraron reparaciones en ese mes.\n";
        }
    } else {
        cout << "Error al abrir el archivo de reparaciones\n";
        system("pause");
        return;
    }
    system("pause");
}

bool estaEsteNumeroEnEsteVector(int num, vector<int> vectorCodigos) {
    for(int i=0; i<vectorCodigos.size(); i++) {
        if (num==vectorCodigos[i]) {
            return true;
        }
    }
    return false; // retornará falso si no se encontrara ese numero en el vector de códigos
}

bool existeElProducto (string nombreArchivoProductos, int codigoProducto) {
    ifstream archivo;
    DatosProducto producto;
    archivo.open(nombreArchivoProductos, ios::binary);
    if (archivo.good()) {
        while(archivo.read((char*)(&producto), sizeof(DatosProducto))) {
            if (producto.codigo == codigoProducto && producto.eliminado==false) {
                return true;
            }
        }
    } else {
        cout << "No se pudo abrir el archivo de productos\n";
    }
    archivo.close();
    return false;
}

/*
int codigoProductoMasVecesReparadoDelMes(string nombreArchivoReparaciones, string nombreArchivoProductos, int mes, int anio) {
    ifstream archivoReparaciones;
    Reparaciones reparacion;
    vector<int> codigosTemporalesdelMes = {-1}; //empieza con -1 para que pueda escanear el código la primera vez
    vector<int> reparacionesTemporalSegunCodigo = {0}; // se inicializa igual con un numero para que puedan estar ambos a la par
    int codigoMasReparado;
    int cantidadDeReparacionesMayor = 0;

    archivoReparaciones.open(nombreArchivoReparaciones, ios::binary);
    if (archivoReparaciones.good()) {
        while (archivoReparaciones.read((char*)(&reparacion), sizeof(Reparaciones))) {
            if (anio==reparacion.fechaReparacion.anio && mes==reparacion.fechaReparacion.mes && estaEsteNumeroEnEsteVector(reparacion.codigoProducto,codigosTemporalesdelMes)==false && existeElProducto(nombreArchivoProductos, reparacion.codigoProducto)) {
                codigosTemporalesdelMes.push_back(reparacion.codigoProducto);
                reparacionesTemporalSegunCodigo.push_back(0); // todas las reparaciones están en 0, pero los vectores necestian ser del mismo tamaño
            }
        }
    } else {
        cout << "No se puedo abrir el archivo de Reparaciones\n";
        return -1;
    }
    archivoReparaciones.close();

    // se vuelve a abrir el archivo esta vez para ver entre cada codigo cuantas ventas tienen
    for (int i=0; i<codigosTemporalesdelMes.size(); i++) {
        if (i>0) { // no se toma en cuenta el primer miebro porque es -1
            archivoReparaciones.open(nombreArchivoReparaciones, ios::binary);
            while(archivoReparaciones.read((char*)(&reparacion),sizeof(Reparaciones))) {
                if (reparacion.codigoProducto == i) {
                    reparacionesTemporalSegunCodigo[i]++; // se van sumando según i las reparaciones sin importar su orden
                }
            }
            archivoReparaciones.close();
        }
    }

    //Ahora se compara la cantidad de reparaciones por codigo
    for (int i=0; i<reparacionesTemporalSegunCodigo.size(); i++) {
        if (i>0 && cantidadDeReparacionesMayor<reparacionesTemporalSegunCodigo[i]) {
            cantidadDeReparacionesMayor = reparacionesTemporalSegunCodigo[i];
            codigoMasReparado = codigosTemporalesdelMes[i];
        }
    }
    return codigoMasReparado;
} 
*/

int codigoProductoMasVecesReparadoDelMes(string nombreArchivoReparaciones, string nombreArchivoProductos, int mes, int anio) {
    ifstream archivoReparaciones;
    Reparaciones reparacion;
    vector<int> codigos;
    vector<int> contadores;
    int codigoMasReparado = -1;
    int maxReparaciones = 0;
    bool encontrado = false;

    archivoReparaciones.open(nombreArchivoReparaciones, ios::binary);
    if (archivoReparaciones.good()) {
        while (archivoReparaciones.read((char*)(&reparacion), sizeof(Reparaciones))) {
            if (anio==reparacion.fechaReparacion.anio && mes==reparacion.fechaReparacion.mes) {
                for (int i = 0; i < codigos.size(); i++) {
                    if (codigos[i] == reparacion.codigoProducto) {
                        contadores[i]++;
                        encontrado = true;
                        }
                if (!encontrado) {
                    codigos.push_back(reparacion.codigoProducto);
                    contadores.push_back(1);
                    }
                }
            }
        }
    } else {
        cout << "No se puedo abrir el archivo de Reparaciones\n";
        return -1;
    }
    archivoReparaciones.close();

    for (int i = 0; i < contadores.size(); i++) {
        if (contadores[i] > maxReparaciones) {
            maxReparaciones = contadores[i];
            codigoMasReparado = codigos[i];
        }
    }

    return codigoMasReparado; // -1 si no hubo reparaciones
} 


void reporteProductoMensualMasReparadoPantalla(string nombreArchivoReparaciones, string nombreArchivoProductos) {
    ifstream archivoReparaciones;
    ifstream archivoProductos;
    Reparaciones reparacion;
    DatosProducto producto;
    int anioBuscado;
    int mesBuscado;
    int codigoDelProductoMasVecesReparado;
    int contadorReparaciones = 0;

    cout << "REPORTE MENSUAL DE PRODUCTO MÁS REPARADO\n";
    cout << "Ingrese el año del reporte: ";
    cin >> anioBuscado;
    cout << "Ingrese el mes del reporte: ";
    cin >> mesBuscado;
    codigoDelProductoMasVecesReparado = codigoProductoMasVecesReparadoDelMes(nombreArchivoReparaciones, nombreArchivoProductos, mesBuscado, anioBuscado);
    
    archivoProductos.open(nombreArchivoProductos,ios::binary);
    if (archivoProductos.good()) {
        system("cls");
        cout << "=== REPORTE DEL MES DE " << meses[mesBuscado] << " DEL AÑO " << anioBuscado << " ===\n";
        while (archivoProductos.read((char*)(&producto), sizeof(DatosProducto))) {
            if (producto.codigo == codigoDelProductoMasVecesReparado && producto.eliminado==false) {
                cout << "== PRODUCTO MÁS REPARADO DEL MES ==\n";
                cout << "Código: " << producto.codigo << endl;
                cout << "Modelo: " << producto.modelo << endl;
                cout << "Categoría: " << producto.categoria << endl;
                cout << "Precio: " << producto.precioVenta << endl;
                cout << "Stock: " << producto.stock << endl;
                cout << "=========================\n";
            }
        }
        system("pause");
        archivoProductos.close();
    } else {
        cout << "Error al buscar el producto\n";
        system("pause");
        return;
    }

    archivoReparaciones.open(nombreArchivoReparaciones, ios::binary);
    if (archivoReparaciones.good()) {
        cout << "== REPARACIONES DE ESE MES ==\n";
        cout << "FECHA\tCI\tFACTURA\tPRECIO\n";
        while(archivoReparaciones.read((char*)(&reparacion), sizeof(Reparaciones))) {
            if (reparacion.fechaReparacion.anio==anioBuscado && reparacion.fechaReparacion.mes==mesBuscado && codigoDelProductoMasVecesReparado==reparacion.codigoProducto) {
                cout << reparacion.fechaReparacion.dia << "/" << reparacion.fechaReparacion.mes << "/" << reparacion.fechaReparacion.anio;
                cout << "\t" << reparacion.CI_cliente << "\t" << reparacion.numeroFactura << "\t" << reparacion.precioReparacion << "\n";
            }
        }
    } else {
        cout << "Error al abrir el archivo de Reparaciones\n";
        return;
    }

}

void menuABM_Reparacion(string nombreArchivoBin, string nombreArchivoFactura, string nombreArchivoCliente, string nombreArchivoProducto) {
    int opcion;
    do {
        cout << "== MENÚ REPARACIONES ==\n";
        cout << "\t1. Agregar registro de reparación\n";
        cout << "\t2. Encontrar registro de reparación\n";
        cout << "\t3. Modificar descripción de reaparación\n";
        cout << "\t4. Reporte Mensual: Reparaciones de un mes específico\n";
        cout << "\t5. Mostrar reparaciones\n";
        cout << "\t0. Volver\n";
        cout << "--> ";
        cin >> opcion;
        if (opcion==1) {
            agregarDatosReparacion(nombreArchivoBin,nombreArchivoFactura, nombreArchivoCliente, nombreArchivoProducto);
        } else if (opcion==2) {
            encontrarReparacion(nombreArchivoBin);
        } else if (opcion==3) {
            modificarDatosReparacion(nombreArchivoBin);
        } else if(opcion==4) {
            ReporteMensualReparacionesPantalla(nombreArchivoBin, nombreArchivoCliente, nombreArchivoFactura, nombreArchivoProducto);
        } else if (opcion==5) {
            mostrarReparaciones(nombreArchivoBin);
        } else if (opcion==0) {
            cout << "Volviendo al menú principal...\n";
        }
    } while (opcion!=0);

}


// ============= Reporte Ventas ==============================

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
        if (venta.facturada && !venta.eliminada && venta.fechaDeVenta.dia == fecha.dia && venta.fechaDeVenta.mes == fecha.mes && venta.fechaDeVenta.anio == fecha.anio)
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



void reporteMesMasVentas() {
    ifstream archivo_ventas;
    ventaProducto venta;
    
    // Array para meses (1-12)
    string nombresMeses[] = {"", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio","Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    
    // Contadores para cada mes
    float ventasPorMes[13] = {0}; // Indice 0 no usado
    int cantidadPorMes[13] = {0};
    
    archivo_ventas.open("ventas.bin", ios::binary);
    if (!archivo_ventas.is_open()) {
        cout << "No hay ventas registradas para generar el reporte." << endl;
        return;
    }
    
    // Calcular ventas por mes
    while (archivo_ventas.read((char*)&venta, sizeof(ventaProducto))) {
        if (!venta.eliminada) {
            int mes = venta.fechaDeVenta.mes;
            if (mes >= 1 && mes <= 12) {
                float subtotal = venta.cantidad * venta.precioUnitario;
                ventasPorMes[mes] += subtotal;
                cantidadPorMes[mes] += venta.cantidad;
            }
        }
    }
    archivo_ventas.close();
    
    // Encontrar mes con mas ventas
    int mesTop = 0;
    float maxVentasMes = 0;
    int maxCantidad = 0;
    
    for (int mes = 1; mes <= 12; mes++) {
        if (ventasPorMes[mes] > maxVentasMes) {
            maxVentasMes = ventasPorMes[mes];
            maxCantidad = cantidadPorMes[mes];
            mesTop = mes;
        }
    }
    
    cout << endl << "=== REPORTE: MES CON MAS VENTAS ===" << endl;
    
    if (mesTop > 0) {
        cout << "Mes con mas ventas: " << nombresMeses[mesTop] << endl;
        cout << "Ventas totales del mes: $" << maxVentasMes << endl;
        cout << "Cantidad de productos vendidos: " << maxCantidad << endl;
    } else {
        cout << "No se encontraron ventas registradas." << endl;
    }
    
    // Mostrar ventas por todos los meses
    cout << endl << "VENTAS POR MES:" << endl;
    cout << "==================" << endl;
    
    float totalAnual = 0;
    int totalProductosAnual = 0;
    
    for (int mes = 1; mes <= 12; mes++) {
        if (ventasPorMes[mes] > 0) {
            cout << nombresMeses[mes] << ": $" << ventasPorMes[mes] 
                 << " (" << cantidadPorMes[mes] << " productos)" << endl;
            totalAnual += ventasPorMes[mes];
            totalProductosAnual += cantidadPorMes[mes];
        }
    }
    
    cout << endl << "Total anual: $" << totalAnual << endl;
    cout << "Total productos vendidos anual: " << totalProductosAnual << endl;
}




// ============================== FUNCIONES VENTA ==============================


bool buscar_producto(int codigo, DatosProducto &producto_encontrado, string nombre_archivo)
{
    ifstream archivo_productos;
    bool encontrado = false;

    archivo_productos.open(nombre_archivo, ios::binary);

    if (!archivo_productos.good())
    {
        cout << "No se pudo abrir el archivo de productos." << endl;
        return false;
    }
    while (archivo_productos.read((char*)&producto_encontrado, sizeof(DatosProducto)) && !encontrado)
    {
        if (producto_encontrado.codigo == codigo && !producto_encontrado.eliminado)
        {
            encontrado = true;
        }
    }
    archivo_productos.close();
    return encontrado;
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

            archivo_ventas.write((char*)&venta, sizeof(ventaProducto));

            sumar_stock(venta.codigoProducto, venta.cantidad, nombre_archivo_productos);
            // Se devuelve el stock.
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
    bool producto_encontrado = false;

    cout << "Ingresar número de factura a buscar: ";
    cin >> numero_factura_a_buscar;

    archivo_facturas.open(nombre_archivo_facturas, ios::binary);
    if (!archivo_facturas.good())
    {
        cout << "No se pudo abrir archivo de facturas para ver el detalle." << endl;
        return;
    }

    while (archivo_facturas.read((char*)&factura, sizeof(datosFactura)) && !factura_encontrada)
    {
        if (factura.numeroFactura == numero_factura_a_buscar)
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
            producto_encontrado = false;
            // Se debe reiniciar para la siguiente venta.
            
            subtotal = venta.cantidad * venta.precioUnitario;
            total = total + subtotal;

            archivo_productos.open(nombre_archivo_productos, ios::binary);
            if (!archivo_productos.good())
            {
                cout << "No se pudo abrir el archivo de productos" << endl;
                return;
            }
            
            while (archivo_productos.read((char*)&producto, sizeof(DatosProducto)) && !producto_encontrado)
            {
                if (producto.codigo == venta.codigoProducto && !producto.eliminado)
                {
                    producto_encontrado = true;
                }
            }
            
            if (producto_encontrado)
            {
                cout << venta.codigoProducto << "\t" << producto.categoria << "-" << producto.modelo << "\t" << venta.cantidad << "\t" << venta.precioUnitario << "\t" << subtotal << endl;
            }

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
        cout << "4. Reporte de mes con más ventas" << endl;
        cout << "5. Reporte detallado de mes con más ventas" << endl;
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

            case 5:
                reporteMesMasVentas();
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
        cout << endl;
        cout << "5 Crear factura" << endl;
        cout << "6 Anular factura" << endl;
        cout << "7 Listado de facturas emitidas" << endl;
        cout << "8 Mostrar detalle de factura emitida" << endl;
        cout << endl;
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
                mostrar_detalle_factura(nombre_archivo_facturas, nombre_archivo_ventas, nombre_archivo_productos);
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
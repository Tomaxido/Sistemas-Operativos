import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class threads_find {
    public static class Sopa {
        public int tamanio;
        public String abuscar;
        public String[][] letras;

        public void settamanio(int dato) {
            this.tamanio = dato;
            this.letras = new String[dato][dato];
        }

        public void setpalabra(String dato) {
            this.abuscar = dato;
        }
    }

    public static class Busqueda extends Thread {
        private String[][] letras;
        private String palabra;
        private int inicioX, inicioY;
        private int finX;
        private int finY;
        private int tamanio;
        private String result = null;

        public Busqueda(String[][] letras, String palabra, int inicioX, int inicioY, int finX, int finY, int tamanio) {
            this.letras = letras;
            this.palabra = palabra;
            this.inicioX = inicioX;
            this.inicioY = inicioY;
            this.finX = finX;
            this.finY = finY;
            this.tamanio = tamanio;
        }

        public void run() {
            if (tamanio == palabra.length()) {
                for (int x = inicioX; x <= finX; x++) {
                    for (int y = inicioY; y < finY; y++) {
                        if (buscarHorizontal(x, y)) {
                            result = "encontrada horizontalmente: Columna: "+(y+1) + " y Fila: " + (x+1);
                        }
                    }
                }
                for (int x = inicioX; x < finX; x++) {
                    for (int y = inicioY; y <= finY; y++) {
                        if (buscarVertical(x, y)) {
                            result = "encontrada verticalmente: Columna: "+(y+1) + " y Fila: " + (x+1);
                        }
                    }
                }
            } else {
                int new_tamanio = tamanio / 2;
                Busqueda[] cuadrantes = new Busqueda[4];
                cuadrantes[0] = new Busqueda(letras, palabra, inicioX, inicioY, (finX + inicioX) / 2, (finY + inicioY) / 2, new_tamanio);
                cuadrantes[1] = new Busqueda(letras, palabra, inicioX, (finY + inicioY) / 2 + 1, (finX + inicioX) / 2, finY, new_tamanio);
                cuadrantes[2] = new Busqueda(letras, palabra, (finX + inicioX) / 2 + 1, inicioY, finX, (finY + inicioY) / 2, new_tamanio);
                cuadrantes[3] = new Busqueda(letras, palabra, (finX + inicioX) / 2 + 1, (finY + inicioY) / 2 + 1, finX, finY, new_tamanio);

                Thread[] threads = new Thread[4];
                for (int i = 0; i < 4; i++) {
                    threads[i] = new Thread(cuadrantes[i]);
                    threads[i].start();
                }

                for (int i = 0; i < 4; i++) {
                    try {
                        threads[i].join();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    if (cuadrantes[i].result != null) {
                        result = cuadrantes[i].result;
                        return;
                    }
                }
            }
        }

        private boolean buscarHorizontal(int x, int y) {
            for (int i = 0; i < palabra.length(); i++) {
                if (!letras[x][y + i].equalsIgnoreCase(String.valueOf(palabra.charAt(i)))) {
                    return false;
                }
            }
            return true;
        }

        private boolean buscarVertical(int x, int y) {
            for (int i = 0; i < palabra.length(); i++) {
                if (!letras[x + i][y].equalsIgnoreCase(String.valueOf(palabra.charAt(i)))) {
                    return false;
                }
            }
            return true;
        }
    }

    public static void main(String[] args) {
        System.out.println("----------------------------------------------------------------------------");
        System.out.println("Búsqueda Mediante Hebras Recursivas:");
        Integer total = 0;
        Integer contador = 0;
        File directorio = new File("Laberintos");
        File[] carpetas = directorio.listFiles();
        if (carpetas != null) {
            for (File carpeta : carpetas) {
                if (carpeta.isDirectory()) {
                    Sopa soup = new Sopa();
                    leerArchivo(carpeta, soup);
                    String palabra = soup.abuscar;
                    Busqueda busqueda = new Busqueda(soup.letras, palabra, 0, 0, soup.letras.length - 1, soup.letras[0].length - 1, soup.tamanio);
                    System.out.println("----------------------------------------------------------------------------");
                    System.out.println("Buscando: " + soup.abuscar);
                    long tiempoInicial = System.currentTimeMillis();
                    Thread thread = new Thread(busqueda);
                    thread.start();
                    try {
                        thread.join();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    long tiempoFinal = System.currentTimeMillis();
                    long tiempoTotal = tiempoFinal - tiempoInicial;
                    total += (int) tiempoTotal;
                    contador++;
                    if (busqueda.result != null) {
                        System.out.println(" -Palabra " + busqueda.result);
                        System.out.println(" -Tiempo de ejecución: " + tiempoTotal + " milisegundos");
                    } else {
                        System.out.println("Palabra no encontrada.");
                    }
                }
            }
            System.out.println("----------------------------------------------------------------------------");
            System.out.println(" -Tiempo promedio de ejecución: " + total / contador + " milisegundos");
            System.out.println("----------------------------------------------------------------------------");
        }
    }

    public static void leerArchivo(File carpeta, Sopa lab) {
        File[] archivos = carpeta.listFiles();
        if (archivos != null) {
            for (File archivo : archivos) {
                try (BufferedReader br = new BufferedReader(new FileReader(archivo))) {
                    String linea;
                    int contador = 0;
                    while ((linea = br.readLine()) != null) {
                        if (contador == 0) {
                            lab.settamanio(Integer.parseInt(linea));
                        } else if (contador == 1) {
                            lab.setpalabra(linea);
                        } else {
                            lab.letras[contador - 2] = linea.split(" ");
                        }
                        contador++;
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}

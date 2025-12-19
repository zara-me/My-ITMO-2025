
package org.example;

import java.awt.image.BufferedImage;
import java.awt.Color;
import java.io.File;
import javax.imageio.ImageIO;
import java.util.ArrayList;
import java.util.Scanner;

public class NewtonFractal {

    public static void main(String[] args) throws Exception {

        Scanner scanner = new Scanner(System.in);

        int nIter = 2000;
        int nPointsX = 250;
        int nPointsY = 250;
        double xmin = -1;
        double xmax = 1;
        double ymin = -1;
        double ymax = 1;
        double R_default = 1e-10;
        double C_default_re = -1;
        double C_default_im = 0;

        System.out.print("Введите действительную часть комплексного числа C = ");
        String inputA = scanner.nextLine();
        double a = inputA.isEmpty() ? 0 : Double.parseDouble(inputA);

        System.out.print("Введите мнимую часть комплексного числа C = ");
        String inputB = scanner.nextLine();
        double b = inputB.isEmpty() ? 0 : Double.parseDouble(inputB);

        System.out.print("Введите R = (0.001, 0.00001 и т.д) ");
        String inputR = scanner.nextLine();
        double R = inputR.isEmpty() ? R_default : Double.parseDouble(inputR);

        // Determine C: if user entered 0 for both, use default C
        double C_re = (a == 0 && b == 0) ? C_default_re : a;
        double C_im = (a == 0 && b == 0) ? C_default_im : b;

        System.out.println("C = " + C_re + " + " + C_im + "i, R = " + R);

        generateNewtonFractal(nPointsX, nPointsY, xmin, xmax, ymin, ymax, nIter, C_re, C_im, R);
        scanner.close();
    }

    // Defines the function f(z) = z⁵ + c
    public static Complex f(Complex z, Complex c) {
        return z.pow(5).add(c);
    }

    // Defines the derivative f'(z) = 5z⁴
    public static Complex fdiff(Complex z) {
        return z.pow(4).multiply(5);
    }

    // Performs Newton's method iteration to find roots
    public static Complex buildNewtonFractal(Complex z, Complex c, double r, int maxIter) {
        for (int i = 0; i < maxIter; i++) {
            Complex dz = f(z, c).divide(fdiff(z));  // Calculate f(z)/f'(z)
            if (dz.abs() < r) {  // If change is small, we've converged
                return z;  // Return the found root
            }
            z = z.subtract(dz);  // Newton update: z = z - f(z)/f'(z)
        }
        return null;  // Return null if no convergence
    }

    public static int getRootIndex(ArrayList<Complex> roots, Complex rez, double r) {
        for (int i = 0; i < roots.size(); i++) {
            if (roots.get(i).subtract(rez).abs() < r) return i;
        }
        roots.add(rez);
        return roots.size() - 1;
    }

    public static void generateNewtonFractal(int pointsX, int pointsY, double xmin, double xmax,
                                             double ymin, double ymax, int maxIter,
                                             double cRe, double cIm, double R) throws Exception {

        BufferedImage img = new BufferedImage(pointsX, pointsY, BufferedImage.TYPE_INT_RGB);
        Complex c = new Complex(cRe, cIm);
        ArrayList<Complex> roots = new ArrayList<>();
        Color[] colors = {Color.BLUE, Color.GREEN, Color.RED, Color.YELLOW, Color.BLACK};

        for (int ix = 0; ix < pointsX; ix++) {
            double x0 = xmin + ix * (xmax - xmin) / (pointsX - 1);
            for (int iy = 0; iy < pointsY; iy++) {
                double y0 = ymin + iy * (ymax - ymin) / (pointsY - 1);
                Complex z0 = new Complex(x0, y0);

                Complex rez = buildNewtonFractal(z0, c, R, maxIter);
                int colorIndex = 0;
                if (rez != null) {
                    colorIndex = getRootIndex(roots, rez, R);
                }
                img.setRGB(ix, iy, colors[colorIndex % colors.length].getRGB());
            }
        }

        ImageIO.write(img, "png", new File("newton_fractal.png"));
        System.out.println(" → Сохранено: newton_fractal.png");
    }
}

package org.example;

import java.awt.image.BufferedImage;
import java.awt.Color;
import java.io.File;
import javax.imageio.ImageIO;

public class JuliaSet {

    public static void main(String[] args) throws Exception {

        int width = 800;
        int height = 600;
        double centerX = 0.0;
        double centerY = 0.0;
        double scale = 0.005;

        //4 different C values - each creates completely different Julia set shape
        //
        //4 iteration levels - tests how detail changes with more calculations
        double[][] cValues = {
                {-0.4, 0.6},        // Connected Julia set
                {0.285, 0.01},      // Near Mandelbrot boundary
                {-0.7, 0.27},       // "Apple-like" region
                {-0.8, 0.156}       // Very beautiful region :)
        };

        int[] maxIterations = {50, 100, 200, 400};


        for (double[] cPoint : cValues) {
            for (int maxIter : maxIterations) {
                System.out.println("Множество Жюлиа (c=" + cPoint[0] + "," + cPoint[1] + ", Макс итераций=" + maxIter + ")");
                String outFile = "julia_" + cPoint[0] + "_" + cPoint[1] + "_iter" + maxIter + ".png";
                generateJulia(width, height, centerX, centerY, scale, maxIter, cPoint, outFile);
            }
        }

        System.out.println("\n✅ Все изображения сохранены успешно!");
    }

    public static void generateJulia(int width, int height,
                                     double centerX, double centerY,
                                     double scale, int maxIter,
                                     double[] cPoint, String outFile) throws Exception {

        BufferedImage img = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);

        double halfWidth = (width / 2.0) * scale;
        double halfHeight = (height / 2.0) * scale;
        double minX = centerX - halfWidth;
        double maxX = centerX + halfWidth;
        double minY = centerY - halfHeight;
        double maxY = centerY + halfHeight;

        double cRe = cPoint[0];
        double cIm = cPoint[1];
        //Pixel to Math Conversion
        for (int px = 0; px < width; px++) {
            double x0 = minX + (px / (double)(width - 1)) * (maxX - minX);
            for (int py = 0; py < height; py++) {
                double y0 = minY + (py / (double)(height - 1)) * (maxY - minY);

                //KEY DIFFERENCE from Mandelbrot! Start from Z₀ = (x0, y0)
                double x = x0;
                double y = y0;
                int iter = 0;
                double x2 = x*x;
                double y2 = y*y;

                while (x2 + y2 <= 4.0 && iter < maxIter) {
                    double newX = x2 - y2 + cRe;
                    double newY = 2.0 * x * y + cIm;
                    x = newX;
                    y = newY;
                    x2 = x*x;
                    y2 = y*y;
                    iter++;
                }

                int color;
                if (iter >= maxIter) {
                    color = Color.BLACK.getRGB();
                } else {
                    // Smooth coloring based on escape time
                    double modulus = Math.sqrt(x2 + y2);
                    double mu = iter + 1.0 - Math.log(Math.log(Math.max(modulus, 1e-10))) / Math.log(2.0);
                    double norm = mu / maxIter;
                    float hue = (float)((0.95 + 10 * norm) % 1.0);
                    float saturation = 0.7f;
                    float brightness = (float)(0.4 + 0.6 * Math.min(1.0, norm * 1.2));
                    color = Color.HSBtoRGB(hue, saturation, brightness);
                }

                img.setRGB(px, py, color);
            }
        }

        ImageIO.write(img, "png", new File(outFile));
        System.out.println(" → Сохранено: " + outFile);
    }
}

package org.example;

import java.awt.image.BufferedImage;
import java.awt.Color;
import java.io.File;
import javax.imageio.ImageIO;

public class MandelbrotPNG {

    public static void main(String[] args) throws Exception {

        int width  = 800;
        int height = 600;
        double centerReal = -0.5;
        double centerImag = 0.0;
        double scale = 0.005;
        int maxIter = 500;
        String outFile = "mandelbrot.png";

        // اگر کاربر آرگومان فرستاد از آن استفاده کن
        if (args.length >= 1) width = Integer.parseInt(args[0]);
        if (args.length >= 2) height = Integer.parseInt(args[1]);
        if (args.length >= 4) {
            centerReal = Double.parseDouble(args[2]);
            centerImag = Double.parseDouble(args[3]);
        }
        if (args.length >= 5) scale = Double.parseDouble(args[4]);
        if (args.length >= 6) maxIter = Integer.parseInt(args[5]);
        if (args.length >= 7) outFile = args[6];

        System.out.println("Generating " + width + "x" + height + ", center=(" + centerReal + "," + centerImag + "), scale=" + scale + ", maxIter=" + maxIter);
        BufferedImage img = new BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);

        //Calculates how much of the complex plane we can see
        double halfWidth  = (width  / 2.0) * scale;
        double halfHeight = (height / 2.0) * scale;
        double minX = centerReal - halfWidth;
        double maxX = centerReal + halfWidth;
        double minY = centerImag - halfHeight;
        double maxY = centerImag + halfHeight;

        for (int px = 0; px < width; px++) {
            double x0 = minX + (px / (double)(width - 1)) * (maxX - minX);
            for (int py = 0; py < height; py++) {
                double y0 = minY + (py / (double)(height - 1)) * (maxY - minY);

                //  Start from (0,0)
                double x = 0.0;
                double y = 0.0;
                int iter = 0;
                double x2 = 0.0, y2 = 0.0;
                //|z|<=2
                while (x2 + y2 <= 4.0 && iter < maxIter) {
                    // z = z^2 + c
                    y = 2.0 * x * y + y0;
                    x = x2 - y2 + x0;

                    x2 = x * x;
                    y2 = y * y;

                    iter++;
                }

                int color;
                if (iter >= maxIter) {

                    color = Color.BLACK.getRGB();
                } else {
                    // (smooth coloring)
                    // Calculate the smooth value: iter + 1 - log(log(|z|)) / log(2)
                    double modulus = Math.sqrt(x2 + y2);
                    double mu = iter + 1.0 - Math.log(Math.log(Math.max(modulus, 1e-10))) / Math.log(2.0);

                    // نرمال‌سازی بین 0 و 1
                    double norm = mu / maxIter;
                    float hue = (float)((0.95 + 10 * norm) % 1.0);
                    float saturation = 0.7f;
                    float brightness = (float)(0.4 + 0.6 * Math.min(1.0, norm*1.2));
                    color = Color.HSBtoRGB(hue, saturation, brightness);
                }
                img.setRGB(px, py, color);
            }
        }

        ImageIO.write(img, "png", new File(outFile));
        System.out.println("Saved to " + outFile);
    }
}

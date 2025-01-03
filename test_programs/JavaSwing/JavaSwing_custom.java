import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.Random;

public class JavaSwing_custom extends JFrame {
	private int rect_count = 0;
    private Color color;
    private int HEIGHT;

    public JavaSwing_custom(int rect_count) {
		this.rect_count = rect_count;
        setSize(1920, 1080);
        setTitle("Swing Test Application");
        setExtendedState(JFrame.MAXIMIZED_BOTH);
        setVisible(true);
        color = new Color(0, 0, 0);
    }

    public void paint(Graphics g) {
	    g.setColor(color);
	    g.fillRect(0, 0, getWidth(), getHeight());
    }

    public void mousePressed(MouseEvent e) {
	    HEIGHT = getHeight();
        color = new Color(255, 255, 255);


        Graphics g = getGraphics();

        Random r = new Random();
        for (int i = 0; i < rect_count; i++) {
            int x = 300 + r.nextInt(1920 - 300);
            int y = r.nextInt(HEIGHT);
            int width = r.nextInt(1920 - x);
            int height = r.nextInt(HEIGHT - y);
            int red = r.nextInt(254);
            int green = r.nextInt(255);
            int blue = r.nextInt(255);
            float alpha = 255; //r.nextFloat();
            g.setColor(new Color(red, green, blue));
            g.fillRect(x, y, width, height);
        }
        g.setColor(new Color(255, 255, 255));
        g.fillRect(0, 0, 300, HEIGHT);

        Toolkit.getDefaultToolkit().sync();
    }

    public void mouseReleased(MouseEvent e) {
        color = new Color(0, 0, 0);

        Graphics g = getGraphics();
        if (g != null) {
            g.setColor(color);
            g.fillRect(0, 0, getWidth(), getHeight());
            Toolkit.getDefaultToolkit().sync();
        }
    }

    public static void main(String[] args) {
		int rect_count = 0;

		if (args.length > 0)
		{
			rect_count = Integer.parseInt(args[0]) - 1;
		}

        JavaSwing_custom window = new JavaSwing_custom(rect_count);
        window.addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                window.mousePressed(e);
            }

            public void mouseReleased(MouseEvent e) {
                window.mouseReleased(e);
            }
        });
        GraphicsEnvironment graphics = GraphicsEnvironment.getLocalGraphicsEnvironment();
        GraphicsDevice device = graphics.getDefaultScreenDevice();
        device.setFullScreenWindow(window);
    }
}

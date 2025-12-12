import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.geometry.Insets;
import javafx.geometry.Rectangle2D;
import javafx.scene.Scene;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Screen;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

import java.io.IOException;
import java.util.Random;

public class JavaFX_rects extends Application {
	static int rect_count = 999;

    private int WIDTH = 1920;
    private int HEIGHT = 1080;

	Pane pane;
	Scene scene;
	Rectangle[] rects;
	Random r = new Random();
	//Rectangle2D screenBounds = Screen.getPrimary().getBounds();
	
	//int rect_count = 100;

    //public JavaFX_custom(int rect_count) {
	@Override
    public void start(Stage stage) throws IOException {
		this.rect_count = rect_count;
        //setSize(WIDTH, HEIGHT);
        stage.setTitle("JavaFX Test Application");
		//stage.initStyle(StageStyle.UNDECORATED);
		stage.setFullScreen(true);
		stage.setX(0);
		stage.setY(0);

		rects = new Rectangle[rect_count+1];

		pane = new Pane();
		scene = new Scene(pane, WIDTH, HEIGHT);

		BackgroundFill backgroundFill = new BackgroundFill(Color.BLACK, CornerRadii.EMPTY, Insets.EMPTY);
		Background background = new Background(backgroundFill);
		pane.setBackground(background);

		pane.setOnMousePressed(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                mousePressed();
            }
        });

		pane.setOnMouseReleased(new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent mouseEvent) {
                mouseReleased();
            }
        });

		stage.setScene(scene);
		stage.show();
    }

    public void mousePressed() {
        for (int i = 0; i < rect_count; i++) {
            int x = 300 + r.nextInt(1920 - 300);
            int y = r.nextInt(HEIGHT);
            int width = r.nextInt(1920 - x);
            int height = r.nextInt(HEIGHT - y);
            float red =   r.nextFloat();
            float green = r.nextFloat();
            float blue =  r.nextFloat();
            float alpha = 1;

			Rectangle rect = new Rectangle(x, y, width, height);
			rect.setFill(new Color(red, green, blue, alpha));
			rects[i] = rect;
			pane.getChildren().add(rect);
        }

		Rectangle whiteRect = new Rectangle(0, 0, 300, HEIGHT);
		whiteRect.setFill(new Color(1, 1, 1, 1));
		rects[rect_count] = whiteRect;
		pane.getChildren().add(whiteRect);
    }

    public void mouseReleased() {
		pane.getChildren().removeAll(rects);
    }

    public static void main(String[] args) {
		launch();
    }
}

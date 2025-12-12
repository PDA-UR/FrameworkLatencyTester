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

import javafx.scene.image.Image;
import javafx.scene.image.ImageView;

import java.io.IOException;
import java.util.Random;

public class JavaFX_image extends Application {
    private int WIDTH = 1920;
    private int HEIGHT = 1080;

	Rectangle2D viewport;
	Image image;
	ImageView imageView;

	Pane pane;
	Scene scene;
	Random r = new Random();

	@Override
    public void start(Stage stage) throws IOException {
        stage.setTitle("JavaFX Test Application");
		//stage.initStyle(StageStyle.UNDECORATED);
		stage.setFullScreen(true);
		stage.setX(0);
		stage.setY(0);

		viewport = new Rectangle2D(0, 0, WIDTH, HEIGHT);
		image = new Image("file:noise.png");
		imageView = new ImageView();
		imageView.setImage(image);
		imageView.setVisible(false);
		imageView.setViewport(viewport);


		pane = new Pane();
		scene = new Scene(pane, WIDTH, HEIGHT);


		pane.getChildren().add(imageView);

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
		//whiteRect = new Rectangle(0, 0, WIDTH, HEIGHT);
		//whiteRect.setFill(new Color(1, 1, 1, 1));
		//pane.getChildren().add(whiteRect);
		imageView.setVisible(true);
    }

    public void mouseReleased() {
		//pane.getChildren().remove(whiteRect);
		imageView.setVisible(false);
    }

    public static void main(String[] args) {
		launch();
    }
}

import * as THREE from 'three';
import Plane from './plane';
import Enemy from './enemy';
import Star from './star';
// import { Header } from './render_text.js';

const config = require('./config');

const renderer = new THREE.WebGLRenderer();
const camera = new THREE.PerspectiveCamera(
    45,
    window.innerWidth / window.innerHeight,
    1,
    1000
);
const scene = new THREE.Scene();
let light;

var hud;

var plane = new Plane([0, 0, 50]);
// var bridge = new Bridge([10, 10, 0]);
var enemies = [];
var stars = [];
var time_started = new Date().getTime();
var time_now = time_started;
var enemies_count = 0;

var lives = config.LIVES;
var score = 0;

function init() {
    scene.background = new THREE.Color('black');
    camera.position.set(0, 0, 100);
    camera.lookAt(0, 0, 0);
    renderer.setSize(window.innerWidth, window.innerHeight);
    document.body.appendChild(renderer.domElement);
}

// movement
var keyMap = {
    W: 87,
    S: 83,
    A: 65,
    D: 68,
    Esc: 27,
    Space: 32,
    L: 76,
    K: 75,
    Q: 81,
    X: 88,
};

var keyActive = [];
document.addEventListener('keydown', onDocumentKeyDown, true);
document.addEventListener('keyup', onDocumentKeyUp, true);
function onDocumentKeyDown(event) {
    var keyCode = event.keyCode;
    keyActive[keyCode] = true;
}
function onDocumentKeyUp(event) {
    var keyCode = event.keyCode;
    keyActive[keyCode] = false;
}

function executeKeyAction() {
    var speed = config.MY_SPEED;

    if (keyActive[keyMap['Esc']]) {
        console.log(enemies);
        console.log(plane.bullets.length);
        // console.log(plane.Mesh.rotation);
    }

    if (!plane.Mesh) return;

    if (keyActive[keyMap['W']]) {
        plane.Mesh.position.y += speed;
        // forward
    }
    if (keyActive[keyMap['S']]) {
        plane.Mesh.position.y -= speed;
        // backward
    }
    if (keyActive[keyMap['A']]) {
        plane.Mesh.position.x -= speed;
        plane.Mesh.rotation.y += config.ROT_SPEED;
        plane.Mesh.rotation.y = Math.min(plane.Mesh.rotation.y, 1.57);
        // left
    }
    if (keyActive[keyMap['D']]) {
        plane.Mesh.position.x += speed;
        plane.Mesh.rotation.y -= config.ROT_SPEED;
        plane.Mesh.rotation.y = Math.max(plane.Mesh.rotation.y, -1.57);
        // right
    }
    if (keyActive[keyMap['Q']]) {
        plane.Mesh.position.z -= speed;
        // down
    }
    if (keyActive[keyMap['X']]) {
        plane.Mesh.position.z += speed;
        // up
    }
    if (keyActive[keyMap['L']]) {
        plane.fire();
    }
}

function setLight() {
    light = new THREE.AmbientLight(0xffffff, 0.7);
    scene.add(light);

    // light = new THREE.PointLight(0xffffff, 5); // soft white light
    // light.position.set(0, 0, 40).normalize();
    // light.lookAt(0, 0, 0);
    // scene.add(light);
}

function spawnEnemy() {
    if (
        ((time_now - time_started) / 1000) * config.ENEMY_RATE >=
            enemies_count &&
        plane.Mesh
    ) {
        // console.log('enemy');
        var vFOV = THREE.MathUtils.degToRad(camera.fov); // convert vertical fov to radians
        var height =
            Math.tan(vFOV / 2) * (camera.position.z - plane.Mesh.position.z);
        var width = height * camera.aspect; // visible width

        var new_enemy = new Enemy([
            width * (2 * Math.random() - 1),
            camera.position.y + height,
            plane.Mesh.position.z,
        ]);
        enemies.push(new_enemy);
        enemies_count += 1;
    }
}

function shootEnemy() {
    if (!plane.bullets) return;

    var dead_enemies = [];
    for (var i = enemies.length - 1; i >= 0; i--) {
        var used_bullets = [];
        var enemy = enemies[i];
        for (var j = plane.bullets.length - 1; j >= 0; j--) {
            var b = plane.bullets[j];
            if (b.Mesh && enemy.Mesh) {
                var bullet_box = b.getBox();
                var enemy_box = enemy.getBox();

                if (enemy_box.intersectsBox(bullet_box)) {
                    // console.log('death', time_now);
                    scene.remove(enemy.Mesh);
                    scene.remove(b.Mesh);
                    dead_enemies.push(enemy);
                    used_bullets.push(b);
                    stars.push(
                        new Star([
                            enemy.Mesh.position.x,
                            enemy.Mesh.position.y,
                            enemy.Mesh.position.z,
                        ])
                    );
                }
            }
        }
        plane.bullets = plane.bullets.filter(x => !used_bullets.includes(x));
    }
    enemies = enemies.filter(x => !dead_enemies.includes(x));
}

function kill() {
    if (!plane.Mesh) return;

    var dead_enemies = [];
    enemies.forEach(enemy => {
        if (enemy.Mesh) {
            var enemy_box = enemy.getBox();
            var plane_box = plane.getBox();

            if (enemy_box.intersectsBox(plane_box)) {
                scene.remove(enemy.Mesh);
                dead_enemies.push(enemy);
                lives -= 1;
                console.log(lives);
            }
        }
    });
    enemies = enemies.filter(x => !dead_enemies.includes(x));
}

function collect() {
    if (!plane.Mesh) return;

    var collected_stars = [];
    stars.forEach(star => {
        if (star.Mesh) {
            var star_box = star.getBox();
            var plane_box = plane.getBox();

            if (star_box.intersectsBox(plane_box)) {
                scene.remove(star.Mesh);
                collected_stars.push(star);
                score += 100;
                console.log(score);
            }
        }
    });
    stars = stars.filter(x => !collected_stars.includes(x));
}

function animate() {
    if (lives == 0) {
        return;
    }
    requestAnimationFrame(animate);

    time_now = new Date().getTime();

    // enemies.forEach(e => {
    //     e.rotate();
    // });

    if (plane.Mesh) {
        // plane.Mesh.rotation.y -= 0.02;
        // Mesh.position.z -= 0.1
        plane.checkScreen();
        enemies.forEach(e => {
            e.move(plane.Mesh.position);
        });
    }
    if (plane.bullets) {
        var wasted_bullets = [];
        plane.bullets.forEach(b => {
            b.move();

            var vFOV = THREE.MathUtils.degToRad(camera.fov); // convert vertical fov to radians
            var height =
                Math.tan(vFOV / 2) *
                (camera.position.z - plane.Mesh.position.z); // visible height

            if (b.Mesh && b.Mesh.position.y >= camera.position.y + height) {
                scene.remove(b.Mesh);
                wasted_bullets.push(b);
            }
        });

        plane.bullets = plane.bullets.filter(x => !wasted_bullets.includes(x));
    }

    shootEnemy();
    kill();
    collect();

    executeKeyAction();
    move_camera();
    spawnEnemy();

    Header();

    renderer.render(scene, camera);
}

function move_camera() {
    var cam_speed = config.CAM_SPEED;
    camera.position.y += cam_speed;
    camera.lookAt.y += cam_speed;

    stars.forEach(s => {
        if (s.Mesh) {
            s.Mesh.position.y += cam_speed;
        }
    });

    if (hud) hud.position.y += cam_speed;
}

function Header() {
    const loader = new THREE.FontLoader();

    loader.load('fonts/helvetiker_regular.typeface.json', function (font) {
        scene.remove(hud);
        var display = 'Lives ' + lives + ' and Score ' + score;
        if (lives == 0) {
            display = 'GAME OVER! You scored ' + score;
        }
        const geometry = new THREE.TextGeometry(display, {
            font: font,
            size: 4,
            height: 1,
        });
        const materials = [
            new THREE.MeshPhongMaterial({ color: 0xffffff, flatShading: true }), // front
            new THREE.MeshPhongMaterial({ color: 0xffffff }), // side
        ];

        hud = new THREE.Mesh(geometry, materials);

        hud.position.set(-50, camera.position.y + 30, 0);

        scene.add(hud);
        // camera.add(hud);
    });
}

init();
setLight();

animate();

export { scene, camera };

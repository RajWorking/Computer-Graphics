import * as THREE from 'three';
import { scene, camera } from '.';

export function Header () {
    const loader = new THREE.FontLoader();
    var textMesh1;

    loader.load('fonts/helvetiker_regular.typeface.json', function (font) {
        const geometry = new THREE.TextGeometry('Hello three.js!', {
            font: font,
            size: 10,
            height: 1,
        });
        const materials = [
            new THREE.MeshPhongMaterial({ color: 0xffffff, flatShading: true }), // front
            new THREE.MeshPhongMaterial({ color: 0xffffff }), // side
        ];

        textMesh1 = new THREE.Mesh(geometry, materials);

        textMesh1.position.set(0, 0, 0);

        scene.add(textMesh1);
        // camera.add(textMesh1);
    });
}

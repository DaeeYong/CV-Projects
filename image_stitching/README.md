# image stitching
두 장의 image를 붙여서 한 장으로 만드는 프로그램.

## description
1. 두 장의 이미지를 촬영.
   1. 두 장의 사진은 같은 장소/시간 에서 촬영함
   2. 카메라의 위치는 고정, 방향만 바꾸어 촬영함
2. 두 장의 사진에서 feature point와 descriptor를 계산
3. 두 장에서 얻은 각각의 feature descriptor 집합을 matching
4. match 중 유사도가 높은 match만 골라서 새로운 match set을 만듬
5. 선택된 match를 이용해서 homography를 계산.
6. homography를 이용하여 다른 이미지에 warping.

## demo video
![Demo](./assets/demo.gif)

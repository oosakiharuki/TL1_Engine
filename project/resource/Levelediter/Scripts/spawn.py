import bpy
import os
import bpy.ops

# 出現ポイントのシンボルの読み込み
class MYADDON_OT_spawn_import_symbol(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_spawn_import_symbol"
    bl_label = "出現ポイントシンボルImport"
    bl_description = "出現ポイントのシンボルをImportします"
    
    prototype_object_name = "ProttypePlayerSpawn"
    object_name = "PlayerSpawn"

    def execute(self, context):
        print("出現ポイント_import")
        
        # オブジェクトが重複しない
        spawn_object = bpy.data.objects.get(MYADDON_OT_spawn_import_symbol.prototype_object_name)
        # オブジェクトが空じゃないときキャンセル
        if spawn_object is not None:
            return {'CANCELLED'}
        
        # スクリプトが配置されているディレクトリ
        addon_directory = os.path.dirname(__file__)
        # モデルのファイルパス
        relative_path = "player/player.obj"
        # モデルファイルのフルパス
        full_path = os.path.join(addon_directory, relative_path)

        # モデルファイルの読み込み
        # object import
        bpy.ops.wm.obj_import('EXEC_DEFAULT',filepath=full_path,display_type='THUMBNAIL',forward_axis='Z',up_axis='Y')
        
        # 回転を適用
        bpy.ops.object.transform_apply(location=False,rotation=True,scale=False,properties=False,isolate_users=False)
        
        # アクティブなオブジェクトの取得
        object = bpy.context.active_object
        # オブジェクト名を変更
        object.name = MYADDON_OT_spawn_import_symbol.prototype_object_name
        # オブジェクトの種類を設定
        object["type"] = MYADDON_OT_spawn_import_symbol.object_name

        # メモリ上にはおいておくがシーンでは除外する
        bpy.context.collection.objects.unlink(object)

        return {'FINISHED'}
    


# シンボルの作成
class MYADDON_OT_make_spawn_point(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_make_spawn_point"
    bl_label = "出現ポイントのシンボルを作成"
    bl_description = "出現ポイントのシンボルを作成します"
    bl_options = {'REGISTER','UNDO'}

    def execute(self, context):
        #読み込み済みのコピー完了オブジェクトを検索
        spawn_object = bpy.data.objects.get(MYADDON_OT_spawn_import_symbol.prototype_object_name)

        #読み込んでいない場合
        if spawn_object is None:
            # 読み込みオペレータ実行
            bpy.ops.myaddon.myaddon_ot_spawn_import_symbol('EXEC_DEFAULT')
            # 再検索
            spawn_object = bpy.data.objects.get(MYADDON_OT_spawn_import_symbol.prototype_object_name)

            print("出現ポイントのシンボルを作成します")
            #Blender選択解除
            bpy.ops.object.select_all(action='DESELECT')

            #非表示オブジェクトを複製
            object = spawn_object.copy()

            #複製したオブジェクトを現在のシーンに出現させる
            bpy.context.collection.objects.link(object)

            #オブジェクト名の変更
            object.name = MYADDON_OT_spawn_import_symbol.object_name

            return {'FINISHED'}


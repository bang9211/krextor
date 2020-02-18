
s = box.schema.space.create('session', {id=1, if_not_exists = true})
s:create_index('primary', {unique = true, if_not_exists = true, parts = {1, 'UNSIGNEND'}})
